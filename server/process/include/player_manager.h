#pragma once

#include <sstream>
#include <string>
#include <map>
#include <vector>
#include "player.h"
#include "types.h"
#include "process.h"
#include "config.h"

enum class Status {
	OK,
	ERR,
	TLE,
	END,
};

struct ProbojPlayerConfig {
    const PlayerConfig& config;
	Limits limits;
	bool use_logs = true;
};

class PlayerManager {
public:
	PlayerManager(const std::string&,const std::vector<ProbojPlayerConfig>&);
	std::stringstream read_player(PlayerID, Status&);
	void send_player(PlayerID,const std::string&);
	std::stringstream send_and_read_player(PlayerID, const std::string&, Status&);
	void kill_player(PlayerID);
private:
	struct ProbojPlayer {
		ProbojPlayer() {}
		ProbojPlayer(const ProbojPlayerConfig& conf, const std::string& command) : id(conf.config.id), name(conf.config.name), dir_path(conf.config.dir_path), process(command, conf.limits) {}
		PlayerID id;
		std::string name;
		std::string dir_path;
		Process process;
	};
	std::map<PlayerID, ProbojPlayer> player_map;
};
