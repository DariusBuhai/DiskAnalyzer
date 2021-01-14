#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

    #include <unistd.h>

    /// Settings
    #define DEBUG 0
    #define SHOW_ERRORS 0
    #define ALLOWED_PROCESSES 100
    #define CHECK_IN_INTERVAL 1 // seconds

    #if DEBUG
        #define PID_PATH "../TempData/daemon.pid"
        #define OUTPUT_PATH "../TempData/daemon_output.txt"
        #define INSTRUCTION_PATH "../TempData/daemon_instruction.txt"
    #else
        #define PID_PATH "/Users/dariusbuhai/Desktop/Programs/C/DiskAnalyzer/TempData/daemon.pid"
        #define OUTPUT_PATH "/Users/dariusbuhai/Desktop/Programs/C/DiskAnalyzer/TempData/daemon_output.txt"
        #define INSTRUCTION_PATH "/Users/dariusbuhai/Desktop/Programs/C/DiskAnalyzer/TempData/daemon_instruction.txt"
    #endif

    /// Standards
    #define MAX_FILE_PATH_SIZE 2048
    #define MAX_SUBDIRS 4096

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

#endif //DISKANALYZER_SHARED_H
