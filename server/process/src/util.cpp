#include "util.h"
#include <cstdio>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/capability.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <unistd.h>

#define SECBIT_NOROOT 0x00000001

int write_line(const char *mapping_file,const char* line){
    FILE *map_file = fopen(mapping_file, "w");
    if (map_file == NULL) {
        perror("fopen");
        return -1;
    }

    if(fprintf(map_file, "%s", line) < 0) {
        fclose(map_file);
        perror("fprintf");
        return -1;
    };
    fclose(map_file);

    return 0;
}

int set_limit(__rlimit_resource_t res, int limit) {
    struct rlimit _limit;
    _limit.rlim_cur = limit;
    _limit.rlim_max = limit;
    if(setrlimit(res, &_limit) != 0) {
        perror("setrlimit");
        return 1;
    }
    return 0;
}

pid_t find_proc_pid(pid_t parent) {
    char children[256];
    snprintf(children, sizeof(children), "/proc/%d/task/%d/children", parent, parent);

    FILE *children_file = fopen(children, "r");
    if (children_file == NULL) {
        perror("fopen");
        fprintf(stderr,"Could not open file");
        return -1;
    }

    pid_t pid;
    if(fscanf(children_file, "%d", &pid) != 1) {
        fclose(children_file);
        fprintf(stderr, "Found pid: %d", pid);
        return -1;
    }
    pid_t other;
    if(fscanf(children_file, "%d", &other) == 1) {
        fclose(children_file);
        die("Unexpected child %d", other);
    }
    fclose(children_file);
    return pid;
}

int rm_rf(const char* path) {
    char command[256];
    snprintf(command, sizeof(command), "rm -rf %s", path);
    return system(command);
}

/// @brief Copy the file at source to dest
int file_copy(const char* source, const char* dest) {
    int src_fd = open(source, O_RDONLY);
    if(src_fd == -1) {
        perror("open");
        return -1;
    }
    int dst_fd = openat(AT_FDCWD, dest, O_WRONLY | O_CREAT, 0777);
    if(dst_fd == -1) {
        perror("open");
        close(src_fd);
        return -1;
    }
    sendfile(dst_fd, src_fd, NULL, 0x7ffff000);

    close(dst_fd);
    close(src_fd);

    return 0;
}

/// @brief Redirect stdin, stdout, and stderr to the given file descriptors and close all other file descriptors
int setup_fds(int stdin_fd, int stdout_fd, int stderr_fd) {
    if(dup2(stdin_fd, STDIN_FILENO) == -1) {
        perror("dup2");
        return -1;
    }
    if(dup2(stdout_fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        return -1;
    }
    if(dup2(stderr_fd, STDERR_FILENO) == -1) {
        perror("dup2");
        return -1;
    }
    if(close_range(3, ~0U, CLOSE_RANGE_UNSHARE) != 0) {
        perror("close_range");
        return -1;
    }
    return 0;
}

int setup_cap() {
    if (prctl(PR_SET_SECUREBITS, SECBIT_NOROOT, 0, 0, 0) == -1) {
           perror("prctl");
           return 1;
    }

    if (cap_set_mode(CAP_MODE_NOPRIV) != 0) {
        perror("cap_set_mode");
        return -1;
    }

    cap_t caps = cap_get_proc();
    if (caps == NULL) {
        perror("cap_get_proc");
        return 1;
    }

    // Clear all capabilities from the bounding set
    if (cap_clear(caps) == -1) {
        perror("cap_clear");
        cap_free(caps);
        return 1;
    }

    // Set the bounding set to the cleared capabilities
    if (cap_set_proc(caps) == -1) {
        perror("cap_set_proc");
        cap_free(caps);
        return 1;
    }

    cap_free(caps);

    return 0;
}

char* temp_dir(const char* parent) {
    char templ[256];
    snprintf(templ, sizeof(templ), "%s/manager.XXXXXXX", parent);
    char* dir = mkdtemp(templ);
    chmod(dir, 0775);
    return dir;
}
