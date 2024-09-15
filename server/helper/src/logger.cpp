#include "logger.h"
#include <iostream>

using namespace std;

Logger::Logger(const string& filename, bool no_logger) : no_logger(no_logger) {
	log_file.open(filename.c_str());
	if(!log_file) {
		cerr << "Error: could not open log file " << filename << endl;
		exit(1);
	}
}

void Logger::log(const string& message) {
    if(no_logger) {
        return;
    }
   	log_file << message << "\n";
}
