#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define CHECK_IN_INTERVAL 10

static int last_checked_seconds = 0;

int process_requests(char* request){

    return 0;
}

_Noreturn int run_daemon(){

    printf("Daemon initialized\n");

    while (1) {
        char request[512 + 1];

        int error = process_requests(request);
        if(error)
            printf("Processed with error %d\n", error);
        fflush(stdout);

        int current_time_seconds = time(NULL);
        if (current_time_seconds - last_checked_seconds > CHECK_IN_INTERVAL) {
            /// Any periodic routines here: delete old files etc...
            last_checked_seconds = current_time_seconds;
        }
    }
}