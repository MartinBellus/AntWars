#pragma once

#include <string>
#include <sstream>
#include <signal.h>

#include "process.h"

class PlayerProcess {
public:
	PlayerProcess(const std::string& player_dir, const std::string& log_path); // TODO configuration stuff
	void run();
	void signal(int sig = SIGTERM);

	void send(const std::string&);
	std::stringstream receive();

	bool is_running() const;

private:
	Process process;

	void kill();
};
