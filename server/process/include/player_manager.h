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
	bool use_sandbox = true;
};

class PlayerManager {
public:
	PlayerManager(const std::string&,const std::vector<ProbojPlayerConfig>&);
	std::stringstream read_player(PlayerID, Status&);
	void send_player(PlayerID,const std::string&);
	std::stringstream send_and_read_player(PlayerID, const std::string&, Status&);
	void kill_player(PlayerID);
	void cleanup();
private:
	struct ProbojPlayer {
		ProbojPlayer() {}
		ProbojPlayer(const ProbojPlayerConfig& conf, std::string sandbox_path, const std::string& command) : id(conf.config.id), name(conf.config.name), process({command, conf.config.name}, sandbox_path, conf.limits) {}
		PlayerID id;
		std::string name;
		Process process;
	};
	std::map<PlayerID, ProbojPlayer> player_map;
	std::string sandbox_root;
};
