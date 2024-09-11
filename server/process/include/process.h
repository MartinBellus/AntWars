#pragma once

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string>

struct Limits {
	int time = 1000;
	int memory = 256*1024*1024;
};

enum class ProcessState {
    OK,
    EXC,
    END,
    ERR
};

struct Process {
	pid_t pid;
	FILE* child_stdin, *child_stdout;
	std::string command;
	bool running = false;
	Limits limits;
	Process(std::string command, Limits limit) : command(command), limits(limit) {};
	Process() {};

	void run(const std::string& = "");
	void send(const std::string&);
	std::string read_stdin();
	void send_signal(int);
};

ProcessState check_process(pid_t);
