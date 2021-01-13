#ifndef DISKANALYZER_SHARED_H
#define DISKANALYZER_SHARED_H

    #include <unistd.h>

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

    #define DEBUG 1
    #define ALLOWED_PROCESSES 100
    #define CHECK_IN_INTERVAL 3 // seconds

    struct process_details{
        pid_t pid;
        int priority;
        int status;
        int percentage;
    };

    struct file_details{
        char* path;
        int no_childs, is_dir;
        double percentage;
        LL size;
        struct file_details *childs[5048];
    };

#endif //DISKANALYZER_SHARED_H
