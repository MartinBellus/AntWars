#include "process.h"
#include "isolate.h"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <poll.h>

using namespace std;

int Process::run(const string& log_path) {
    if(running) {
        return 1;
    }
    int pipes[2][2];

    pipe(pipes[0]);
    pipe(pipes[1]);
    int log_fd;
    if(log_path != "") {
        log_fd = open(log_path.c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
    } else {
        log_fd = open("/dev/null", O_WRONLY);
    }

    IsolateArgs args(
        command,
        sandbox_path,
        limits
    );

    pid = run_isolated(args, pipes[0][0], pipes[1][1], log_fd, &status_pipe);

    close(pipes[0][0]);
    close(pipes[1][1]);
    close(log_fd);

    if(pid < 0) {
        close(pipes[0][1]);
        close(pipes[1][0]);
        return -1;
    }

    child_stdin = fdopen(pipes[0][1], "w");
    child_stdout = fdopen(pipes[1][0], "r");

    running = true;
    return 0;
}

void Process::send(const string& data) {
    if(!running) {
        return;
    }
    fprintf(child_stdin, "%s", data.c_str());
    fflush(child_stdin);
}

string Process::read_stdin(atomic<bool>& stop) {
    if(!running) {
        return "";
    }
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    string result;
    struct pollfd poll_fd;
    poll_fd.fd = fileno(child_stdout);
    poll_fd.events = POLLIN;

    while(!stop) {
        int status = poll(&poll_fd, 1, limits.time_limit/3);
        if(status == -1) {
            return "";
        } else if(status == 0) {
            continue;
        }
        while((read = getline(&line, &len, child_stdout) > 0)) {
            read = strlen(line);
            if(line[read - 1] == '\n' && line[0] == '.') {
                stop = true;
                break;
            }
            result += string(line, line + read);
        }
    }
    if(line != NULL)
        free(line);
    return result;
}

void Process::send_signal(int signal) {
    if(!running) {
        return;
    }
    kill(pid, signal);
}

ProcessState Process::check_status() {
    if(!running) {
        return ProcessState::ERR;
    }
    int status;
    if(read(status_pipe, &status, sizeof(status)) < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            return ProcessState::RUN;
        }else {
            return ProcessState::ERR;
        }
    }
    running = false;
    if(WIFEXITED(status)) {
        if(WEXITSTATUS(status) == 0)
            return ProcessState::END;
    }
    return ProcessState::EXC;
}
