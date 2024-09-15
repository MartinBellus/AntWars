#pragma once

#include "isolate.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string>
#include <atomic>

enum class ProcessState {
    RUN,
    EXC,
    END,
    ERR
};

struct Process {
	pid_t pid;
	FILE* child_stdin, *child_stdout;
	int status_pipe;
	std::vector<std::string> command;
	std::string sandbox_path;
	bool running = false;
	Limits limits;
	Process(std::vector<std::string> command, std::string sandbox_path, Limits limit) : command(command), sandbox_path(sandbox_path), limits(limit) {};
	Process() {};

	int run(const std::string& = "");
	void send(const std::string&);
	std::string read_stdin(std::atomic<bool>&);
	void send_signal(int);
	ProcessState check_status();
};
