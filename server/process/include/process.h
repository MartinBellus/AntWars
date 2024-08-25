#pragma once

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string>

struct Limits {
	int time, memory;
};

struct Process {
	pid_t pid;
	FILE* child_stdin, *child_stdout;
	std::string command;
	bool running = false;
	Limits limit;
	Process(std::string command, Limits limit={500,500}) : command(command), limit(limit) {};

	void run();
	void send(std::string);
	std::string receive();
	void signal(int = SIGTERM);
};
