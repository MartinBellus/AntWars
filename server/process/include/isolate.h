#pragma once

#include <sched.h>
#include <vector>
#include <string>

struct Limits {
    int memory_limit = 256*1024*1024;
    int time_limit = 1000;
    int fsize = 1024*1024;
    int core_limit = 1024*1024;
    int proc_limit = 1;
};

struct IsolateArgs {
    Limits limits;
    std::string sandbox_dir;
    std::vector<std::string> cmd;
    IsolateArgs(std::vector<std::string> cmd, std::string sandbox_dir, Limits limits) : cmd(cmd), sandbox_dir(sandbox_dir), limits(limits) { }
};


///@brief Run a process in a isolated environment
///@param args Arguments for the isolated environment
///@param stdin_fd File descriptor for stdin
///@param stdout_fd File descriptor for stdout
///@param stderr_fd File descriptor for stderr
///@param &status This will be set to the file descriptor of the status pipe
///@return The pid of the process
pid_t run_isolated(IsolateArgs,int, int, int, int*);
