#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

    #include <unistd.h>

    #ifdef WINDOWS
        #include <direct.h>
        #define CURRENT_DIR _getcwd
    #else
        #include <unistd.h>
        #define CURRENT_DIR getcwd
    #endif

    /// Settings
    #define DEBUG
    //#define SHOW_ERRORS
    #define ALLOWED_PROCESSES 100
    #define CHECK_IN_INTERVAL 1 // seconds

    /// Signals paths
    #define PID_PATH "/TempData/daemon.pid"
    #define OUTPUT_PATH "/TempData/daemon_output.txt"
    #define INSTRUCTION_PATH "/TempData/daemon_instruction.txt"

    /// Standards
    #define MAX_FILE_PATH_SIZE 2048
    #define MAX_SUBDIRS 4096
    #define FILENAME_MAX 1024
    #define MAX_PID_SIZE 10

    #define LL long long
    #define LD long double

    #define HIGH 3
    #define MEDIUM 2
    #define LOW 1

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
    struct signal_details{
        int type, priority;
        pid_t pid, ppid;
        char* path;
        void *response;
    };

    struct process_details{
        pid_t pid;
        int priority, status;
        int done_tasks, total_tasks;
        struct file_details* response;
        char *path;
    };

    struct file_details{
        char* path;
        int childs_counter, is_dir;
        double usage;
        LL size;
        struct file_details *childs[MAX_SUBDIRS];
    };

    /// Methods
    LD bytes_to_xb(LL, int);
    void write_to_file(char*, char data[]);
    char* read_from_file(char*);
    char* get_literal_priority(int);
    char* get_current_path();

#endif //DISKANALYZER_SHARED_H
