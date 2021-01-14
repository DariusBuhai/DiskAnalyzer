#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

    #include <unistd.h>

    /// Settings
    #define DEBUG 1
    #define SHOW_ERRORS 0
    #define ALLOWED_PROCESSES 100
    #define CHECK_IN_INTERVAL 3 // seconds


    /// Data types
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
    #define PAUSE 2
    #define RESUME 3
    #define KILL 4

    struct signal_details{
        int type, priority;
        pid_t pid;
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
        struct file_details *childs[5048];
    };

    /// Methods
    LD bytes_to_xb(LL, int);

#endif //DISKANALYZER_SHARED_H
