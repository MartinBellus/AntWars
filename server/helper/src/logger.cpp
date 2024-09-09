#include "logger.h"
#include <iostream>

using namespace std;

Logger::Logger(const string& filename) {
	log_file.open(filename.c_str());
	if(!log_file) {
		cerr << "Error: could not open log file " << filename << endl;
		exit(1);
	}
}

void Logger::log(const string& message) {
	log_file << message << endl;
}
