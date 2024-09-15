#include "player_manager.h"
#include "util.h"
#include <csignal>
#include <future>
#include <iostream>
#include <sys/wait.h>

using namespace std;


static const char* _sandbox_root = nullptr;

void signal_handler(int signum) {
    if(signum == SIGCHLD) {
        waitpid(-1, NULL, WNOHANG);
    }
    if(signum == SIGTERM || signum == SIGINT) {
        if(_sandbox_root != nullptr)
            rm_rf(_sandbox_root);
        exit(1);
    }
}

void setup_signals() {
    // Reap terminating child processes
    signal(SIGCHLD, signal_handler);
    // Ignore SIGPIPE
    signal(SIGPIPE, signal_handler);
    // Cleanup after SIGTERM or SIGINT
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
}

PlayerManager::PlayerManager(const string& home_dir,const vector<ProbojPlayerConfig>& player_configs) {
    sandbox_root = temp_dir("/tmp");
    _sandbox_root = sandbox_root.c_str();
    for(ProbojPlayerConfig proboj_config : player_configs) {
        const PlayerConfig& config = proboj_config.config;
        // Create new player
        if(player_map.find(config.id) != player_map.end()) {
            cerr << "Player with id " << int(config.id) << " already exists!" << endl;
            continue;
        }
        string command = config.dir_path + "/player";
        string sandbox_dir = "";
        if(config.use_sandbox) {
            sandbox_dir = temp_dir(sandbox_root.c_str());
            string destination = sandbox_dir + "/player";
            if(file_copy(command.c_str(), destination.c_str()) != 0) {
                perror("file_copy");
                cerr << "Error copying player to sandbox" << endl;
                continue;
            }
            command = "./player";
        }
        ProbojPlayer player = ProbojPlayer(proboj_config, sandbox_dir, command);

        // Create players log file
        string log_file_path = "";
        if(proboj_config.use_logs) {
            log_file_path = home_dir + "/logs/" + config.name + ".log";
        }

        // Start player process
        if(player.process.run(log_file_path) < 0) {
            cerr << "Error starting player: " << config.name << endl;
            continue;
        }
        player.process.send_signal(SIGSTOP);
        player_map[config.id] = std::move(player);
    }

    // Register signal handlers
    setup_signals();
}

stringstream PlayerManager::read_player(PlayerID id, Status& status) {
    if(player_map.find(id) == player_map.end()) {
        cerr << "Player with id " << int(id) << " does not exist!" << endl;
        return stringstream();
    }
    ProbojPlayer& player = player_map[id];
    atomic<bool> stop_flag(false);
    future<string> response = async(launch::async, [&player, &stop_flag] {
        return player.process.read_stdin(stop_flag);
    });

    player.process.send_signal(SIGCONT);
    auto response_status = response.wait_for(chrono::milliseconds(player.process.limits.time_limit));
    if (response_status == future_status::timeout) {
        status = Status::TLE;
        cerr << "Player " << player.name << " exceeded time limit" << endl;
        stop_flag = true;
        player.process.send_signal(SIGTERM);
        return stringstream();
    }
    ProcessState state = player.process.check_status();
    switch(state) {
        case ProcessState::RUN:
            player.process.send_signal(SIGSTOP);
            status = Status::OK;
            return stringstream(response.get());
        case ProcessState::ERR:
            status = Status::ERR;
            cerr << "Error when reading from player: " << player.name << endl;
            break;
        case ProcessState::EXC:
            status = Status::ERR;
            cerr << "Player " << player.name << " terminated with error" << endl;
            break;
        case ProcessState::END:
            status = Status::END;
            cerr << "Player " << player.name << " terminated too soon" << endl;
            break;
    }

    return stringstream();
}

void PlayerManager::send_player(PlayerID id, const string& data) {
    if(player_map.find(id) == player_map.end()) {
        cerr << "Player with id " << int(id) << " does not exist!" << endl;
        return;
    }
    ProbojPlayer& player = player_map[id];
    ProcessState state = player.process.check_status();
    if(state == ProcessState::RUN) {
        player.process.send(data);
    } else {
        cerr << "Player with id " << int(id) << " is not running!" << endl;
    }
}

stringstream PlayerManager::send_and_read_player(PlayerID id, const string& data, Status& status) {
    if(player_map.find(id) == player_map.end()) {
        cerr << "Player with id " << int(id) << " does not exist!" << endl;
        return stringstream();
    }
    send_player(id, data);
    return read_player(id, status);
}

void PlayerManager::kill_player(PlayerID id) {
    if(player_map.find(id) == player_map.end()) {
        return;
    }
    ProbojPlayer& player = player_map[id];
    ProcessState state = player.process.check_status();
    if(state == ProcessState::RUN) {
        player.process.send_signal(SIGTERM);
    }
    cerr << "Killing player " << player.name << endl;
    player_map.erase(id);
}

void PlayerManager::cleanup() {
    // remove sandbox directory
    if(rm_rf(sandbox_root.c_str()) != 0) {
        cerr << "Could not remove directory: " << sandbox_root << endl;
    }

    // stop all processes
    for(auto &[id, player] : player_map) {
        player.process.send_signal(SIGKILL);
    }
}
