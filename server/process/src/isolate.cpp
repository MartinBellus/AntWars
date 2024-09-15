#include "isolate.h"
#include "util.h"
#include <csignal>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <linux/capability.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>

struct CmdArgs {
    const char* sanbox_dir;
    bool use_chroot;
    int argc;
    char** cmd;
    int stdin_fd, stdout_fd, stderr_fd;
    Limits limits;
};

struct ProxyArgs {
    int prep_pipe[2];
    int status_pipe[2];
    CmdArgs* cmd_args;
};

void set_limits(Limits l) {
    set_limit(RLIMIT_AS, l.memory_limit);
    set_limit(RLIMIT_FSIZE, l.fsize);
    set_limit(RLIMIT_CORE, l.core_limit);
    set_limit(RLIMIT_NPROC, l.proc_limit);
    set_limit(RLIMIT_MEMLOCK, 0);
}

void exec_cmd(CmdArgs* args) {
    // set chroot trap
    //
    if(args->use_chroot) {
        if(chroot(args->sanbox_dir) != 0) {
            perror("chroot");
            exit(1);
        }
        chdir("/");
    }
    // set fds
    //
    if(setup_fds(args->stdin_fd, args->stdout_fd, args->stderr_fd) != 0) {
        exit(1);
    }
    // set limits
    //
    set_limits(args->limits);
    // drop capabilities
    //
    if(setup_cap() != 0) {
        exit(1);
    };

    execvp(args->cmd[0], args->cmd);
    perror("execvp");
    exit(1);
}

int proxy_fn(void* _args) {
    ProxyArgs* args = (ProxyArgs*) _args;
    close(args->prep_pipe[1]);
    close(args->status_pipe[0]);

    // exit on parent exit
    if(prctl(PR_SET_PDEATHSIG, SIGTERM) != 0) {
        perror("prctl");
    }

    char buf[32];
    if(read(args->prep_pipe[0], buf, 1) != 1) {
        perror("read");
        return 1;
    }
    // setup complete

    if(setuid(0) != 0) {
        perror("setuid");
        return 1;
    }
    if(setgid(0) != 0) {
        perror("setgid");
        return 1;
    }

    pid_t child_pid = fork();
    if(child_pid == 0) {
        // child
        close(args->prep_pipe[0]);
        close(args->status_pipe[1]);

        // exit on parent exit
        if(prctl(PR_SET_PDEATHSIG, SIGKILL) != 0) {
            perror("prctl");
            return 1;
        }

        exec_cmd(args->cmd_args);
    } else if(child_pid == -1) {
        perror("fork");
        return 1;
    }

    // signal child running
    if(write(args->status_pipe[1], &child_pid, sizeof(child_pid)) != sizeof(child_pid)) {
        perror("proxy write");
        return 1;
    }

    // wait for child to finish and send signal back
    int stat;
    pid_t pid = waitpid(child_pid, &stat, 0);
    if(pid != child_pid) {
        perror("waitpid");
        return 1;
    }
    write(args->status_pipe[1], &stat, sizeof(stat));

    close(args->status_pipe[1]);
    close(args->prep_pipe[0]);
    free(args->cmd_args->cmd);

    return 0;
}

void prepare_user_ns(pid_t pid) {
    char file[256], buf[32];
    sprintf(file, "/proc/%d/uid_map", pid);
    sprintf(buf, "0 %d 1\n", getuid());

    if(write_line(file, buf) == -1) {
        perror("write error");
        exit(1);
    }

    sprintf(file, "/proc/%d/setgroups", pid);
    sprintf(buf, "deny\n");
    if(write_line(file, buf) == -1) {
        perror("write error");
        exit(1);
    }

    sprintf(file, "/proc/%d/gid_map", pid);
    sprintf(buf, "0 %d 1\n", getgid());
    if(write_line(file, buf) == -1) {
        perror("write error");
        exit(1);
    }
}

const int STACK_SIZE = 1024 * 1024;

pid_t run_isolated(IsolateArgs args, int stdin_fd, int stdout_fd, int stderr_fd, int* status_fd) {
    char* stack = (char*) malloc(STACK_SIZE);

    CmdArgs cmd_args{
        .sanbox_dir = args.sandbox_dir.c_str(),
        .use_chroot = args.sandbox_dir.size() != 0,
        .argc = (int) args.cmd.size(),
        .cmd = (char**) malloc((args.cmd.size()+1)*sizeof(char*)),
        .stdin_fd = stdin_fd, .stdout_fd = stdout_fd, .stderr_fd = stderr_fd,
        .limits = args.limits
    };
    ProxyArgs proxy_args;

    // copy cmd
    for(int i = 0;i < args.cmd.size(); i++) {
        cmd_args.cmd[i] = strdup(args.cmd[i].c_str());
    }
    cmd_args.cmd[args.cmd.size()] = NULL;
    proxy_args.cmd_args = &cmd_args;

    // create pipes
    if(pipe(proxy_args.prep_pipe) != 0) {
        die("Can not create pipe");
    }
    if(pipe(proxy_args.status_pipe) != 0) {
        die("Can not create pipe");
    }

    *status_fd = proxy_args.status_pipe[0];

    int flags = CLONE_NEWPID | CLONE_NEWIPC | CLONE_NEWNS | CLONE_NEWUSER | SIGCHLD;
    pid_t proxy_pid = clone(proxy_fn, stack + STACK_SIZE, flags, &proxy_args);
    close(proxy_args.prep_pipe[0]);
    close(proxy_args.status_pipe[1]);

    // setup namespaces
    prepare_user_ns(proxy_pid);

    // signal prep_end to proxy
    write(proxy_args.prep_pipe[1], "1", 1);

    // wait for proxy to finish setup
    pid_t child_pid_inside;
    if(read(proxy_args.status_pipe[0], &child_pid_inside, sizeof(child_pid_inside)) != sizeof(child_pid_inside)) {
        fprintf(stderr, "Unexpected data from proxy: %d", child_pid_inside);
        return -1;
    }
    pid_t real_pid = find_proc_pid(proxy_pid);
    if(real_pid <= 0) {
        perror("get_proc_pid");
        return -1;
    }

    // set status_fd to non-blocking
    flags = fcntl(*status_fd, F_GETFL, 0);
    if(fcntl(*status_fd, F_SETFL, flags | O_NONBLOCK) != 0) {
        perror("fcntl");
        return -1;
    }

    return real_pid;
}
