#include "player_manager.h"
#include <iostream>

using namespace std;

PlayerManager::PlayerManager(string home_dir, vector<PlayerConfig>& player_configs) {
	for(PlayerConfig config : player_configs) {
		// Create new player
		string command = config.dir_path + "/player";
		ProbojPlayer player = ProbojPlayer(config.id, config.name, config.dir_path, Process(command));
		if(player_map.find(config.id) != player_map.end()) {
			cerr << "Player with id " << int(config.id) << " already exists!" << endl;
			continue;
		}
		player_map[config.id] = player;

		// Create players log file
		string log_file_path = home_dir + "/logs/" + config.name + ".log";
		FILE* log_file = fopen(log_file_path.c_str(), "w");
		fclose(log_file);

		// Start player process
		player.process.run();
		player.process.signal(SIGSTOP);
	}
}
