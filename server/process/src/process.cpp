#include "process.h"
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>

using namespace std;

void set_limits(Limits limits) {
    struct rlimit limit;
    limit.rlim_cur = limits.memory;
    limit.rlim_max = limits.memory;
    if(setrlimit(RLIMIT_AS, &limit) != 0) {
       perror("setrlimit");
       exit(1);
    }
}

// TODO add better isolation
void Process::run(const string& log_path) {
    if(running) {
        return;
    }
    int pipes[2][2];

    pipe(pipes[0]);
    pipe(pipes[1]);

    if((pid = fork()) > 0){

        close(pipes[0][0]);
        close(pipes[1][1]);

        child_stdin = fdopen(pipes[0][1], "w");
        child_stdout = fdopen(pipes[1][0], "r");
    } else{
        close(pipes[1][0]);
        close(pipes[0][1]);


        dup2(pipes[0][0], STDIN_FILENO);
        dup2(pipes[1][1], STDOUT_FILENO);

        if(log_path != "") {
            int log_fd = open(log_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            dup2(log_fd, STDERR_FILENO);
            close(log_fd);
        }

        close(pipes[0][0]);
        close(pipes[1][1]);

        set_limits(limits);
        char* args[] = {(char*)command.c_str(), NULL};
        execvp(command.c_str(), args);

        exit(1);
    }
    running = true;
}

void Process::send(const string& data) {
    if(!running) {
        return;
    }
    fprintf(child_stdin, "%s", data.c_str());
    fflush(child_stdin);
}

string Process::read_stdin() {
    if(!running) {
        return "";
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    string result;
    while((read = getline(&line, &len, child_stdout)) != -1) {
        if(line[read - 1] == '\n' && line[0] == '.') {
            break;
        }
        result += string(line, line + read);
    }
    return result;
}

void Process::send_signal(int signal) {
    if(!running) {
        return;
    }
    kill(pid, signal);
}

ProcessState check_process(pid_t pid) {
    int status;
    pid_t ret_pid;
    if((ret_pid = waitpid(pid, &status, WNOHANG)) == 0) {
        // process is still running
        return ProcessState::OK;
    }else if(ret_pid == -1) {
        // process does not exist
        return ProcessState::ERR;
    }
    if(WIFEXITED(status)) {
        // process ended normally, but too soon
        return ProcessState::END;
    }else {
        // process ended abnormally
        return ProcessState::EXC;
    }
}
