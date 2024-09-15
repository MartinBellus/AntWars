#pragma once

#include <string>
#include <fstream>

class Logger {
public:
	Logger(const std::string&, bool no_logger = false);
	void log(const std::string&);
private:
	std::ofstream log_file;
	bool no_logger;
};
