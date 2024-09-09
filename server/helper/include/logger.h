#pragma once

#include <string>
#include <fstream>

class Logger {
public:
	Logger(const std::string&);
	void log(const std::string&);
private:
	std::ofstream log_file;
};
