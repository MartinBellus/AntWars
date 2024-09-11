#include "player_manager.h"
#include <future>
#include <iostream>
#include <sys/wait.h>

using namespace std;

// TODO add signal handlers to cleanup processes on exit
PlayerManager::PlayerManager(const string& home_dir,const vector<ProbojPlayerConfig>& player_configs) {
    for(ProbojPlayerConfig proboj_config : player_configs) {
        const PlayerConfig& config = proboj_config.config;
        // Create new player
        if(player_map.find(config.id) != player_map.end()) {
            cerr << "Player with id " << int(config.id) << " already exists!" << endl;
            continue;
        }
        string command = config.dir_path + "/player";
        ProbojPlayer player = ProbojPlayer(proboj_config, command);

        // Create players log file
        string log_file_path = "";
        if(proboj_config.use_logs) {
            log_file_path = home_dir + "/logs/" + config.name + ".log";
        }

        // Start player process
        player.process.run(log_file_path);
        player.process.send_signal(SIGSTOP);
        player_map[config.id] = std::move(player);
    }
}

stringstream PlayerManager::read_player(PlayerID id, Status& status) {
    if(player_map.find(id) == player_map.end()) {
        cerr << "Player with id " << int(id) << " does not exist!" << endl;
        return stringstream();
    }
    ProbojPlayer& player = player_map[id];
    future<string> response = async(launch::async, [&player] {
        return player.process.read_stdin();
    });

    player.process.send_signal(SIGCONT);
    auto response_status = response.wait_for(chrono::milliseconds(player.process.limits.time));
    if (response_status == future_status::timeout) {
        status = Status::TLE;
        kill_player(id);
        cerr << "Player " << player.name << " exceeded time limit" << endl;
        return stringstream();
    }
    ProcessState state = check_process(player.process.pid);
    switch(state) {
        case ProcessState::OK:
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
    kill_player(id);

    return stringstream();
}

void PlayerManager::send_player(PlayerID id, const string& data) {
    if(player_map.find(id) == player_map.end()) {
        cerr << "Player with id " << int(id) << " does not exist!" << endl;
        return;
    }
    ProbojPlayer& player = player_map[id];
    ProcessState state = check_process(player.process.pid);
    if(state == ProcessState::OK) {
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
    ProcessState state = check_process(player.process.pid);
    if(state == ProcessState::OK) {
        player.process.send_signal(SIGTERM);
        waitpid(player.process.pid, NULL, WNOHANG);
    }
    cerr << "Killing player " << player.name << endl;
    player_map.erase(id);
}
