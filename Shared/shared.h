#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

#ifdef WINDOWS
    #include <direct.h>
    #define CURRENT_DIR _getcwd
#else
    #include <unistd.h>
    #define CURRENT_DIR getcwd
#endif

/// Settings
#define DEBUG
#define SHOW_ERRORS
#define ALLOWED_PROCESSES 5
#define CHECK_IN_INTERVAL 1

/// Signals paths
#ifdef LINUX
    #define PID_PATH "/TempData/daemon.pid"
    #define OUTPUT_PATH "/TempData/daemon_output.txt"
    #define INSTRUCTION_PATH "/TempData/daemon_instruction.txt"
    #define ANALYSIS_PATH "/TempData/analysis_%d"
    #define STATUS_PATH "/TempData/status_%d"
#else
    #define PID_PATH "/../TempData/daemon.pid"
    #define OUTPUT_PATH "/../TempData/daemon_output.txt"
    #define INSTRUCTION_PATH "/../TempData/daemon_instruction.txt"
    #define ANALYSIS_PATH "/../TempData/analysis_%d"
    #define STATUS_PATH "/../TempData/status_%d"
#endif


/// Standards
#define MAX_SUBDIRS 4096
#define MAX_PID_SIZE 10
#define MAX_PATH_LENGTH 1024

#define LL long long
#define LD long double

#define HIGH 3
#define MEDIUM 2
#define LOW 1

#define T_PENDING 0
#define T_IN_PROGRESS 1
#define T_PAUSED 2
#define T_DONE 3
#define T_REMOVED 5

#define DONE 1
#define RUNNING 2
#define PAUSED 3
#define FORCE_PAUSED 4
#define KILLED 5

#define ADD 1
#define SUSPEND 2
#define RESUME 3
#define KILL 4
#define INFO 5
#define LIST 6
#define PRINT 7

/// Data types
struct signal_details {
    int type, priority;
    pid_t pid, ppid;
    char* path;
};

struct process_details {
    pid_t pid;
    int priority, status;
    int done_tasks, total_tasks;
    struct file_details* response;
    char *path;
};

struct file_details {
    char* path;
    int childs_counter, is_dir;
    double usage;
    LL size;
    struct file_details *childs[MAX_SUBDIRS];
};

struct task_details {
  int task_id;
  char path[1024];
  int status; // 0 - pending, 1 - in progress, 2 - paused, 3 - done, 4 - removed
  int priority;
  int worker_pid;
};

/// Methods
LD bytes_to_xb(LL, int);
int fsize(FILE *fp);
void write_to_file(const char*, const char* data);
char* read_from_file(const char*);
char* get_literal_priority(int);
char* get_literal_status(int);
char* get_current_path();

#endif //DISKANALYZER_SHARED_H
