#pragma once

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "player.h"
#include "types.h"
#include "process.h"

enum class Status {
	Ok,
	Err,
	Tle,
};

struct PlayerConfig {
	PlayerConfig(std::string name, std::string dir_path) : id(++id_counter), name(name), dir_path(dir_path) {}
	std::string name;
	std::string dir_path;
	PlayerID id;
	static inline PlayerID id_counter = PlayerID(0);
};

class PlayerManager {
public:
	PlayerManager(std::string, std::vector<PlayerConfig>&);
	std::stringstream read_player(PlayerID, Status&);
	void send_player(PlayerID, std::string); // TODO: change string to something
	std::stringstream send_and_read_player(PlayerID, std::string, Status&);
	void log_player(PlayerID, std::string);
	void kill_player(PlayerID);
private:
	struct ProbojPlayer {
		ProbojPlayer() {};
		PlayerID id;
		std::string name;
		std::string dir_path;
		Process process;
	};
	std::map<PlayerID, ProbojPlayer> player_map;
};
