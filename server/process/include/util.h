#pragma once

#include <cstdlib>
#include <cstdio>
#include <sys/resource.h>

#define die(...) fprintf(stderr, __VA_ARGS__); exit(1);

int write_line(const char*,const char*);
int set_limit(__rlimit_resource_t, int);
pid_t find_proc_pid(pid_t);
int rm_rf(const char*);
int file_copy(const char*, const char*);
int setup_fds(int,int,int);
int setup_cap();
char* temp_dir(const char*);
