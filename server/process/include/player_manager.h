#pragma once

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "player.h"
#include "types.h"
#include "process.h"

enum class Status {
	OK,
	ERR,
	TLE,
	END,
};

struct PlayerConfig {
	PlayerConfig(std::string name, std::string dir_path, Limits limits) : id(++id_counter), name(name), dir_path(dir_path), limits(limits) {}
	std::string name;
	std::string dir_path;
	PlayerID id;
	Limits limits;
	bool use_logs;
	static inline PlayerID id_counter = PlayerID(0);
};

class PlayerManager {
public:
	PlayerManager(std::string, std::vector<PlayerConfig>&);
	std::stringstream read_player(PlayerID, Status&);
	void send_player(PlayerID,const std::string&);
	std::stringstream send_and_read_player(PlayerID, const std::string&, Status&);
	void kill_player(PlayerID);
private:
	struct ProbojPlayer {
		ProbojPlayer() {}
		ProbojPlayer(const PlayerConfig& conf, const std::string& command) : id(conf.id), name(conf.name), dir_path(conf.dir_path), process(command, conf.limits) {}
		PlayerID id;
		std::string name;
		std::string dir_path;
		Process process;
	};
	std::map<PlayerID, ProbojPlayer> player_map;
};
