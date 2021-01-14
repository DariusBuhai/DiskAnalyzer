#include <stdio.h>
#include <time.h>
#include <sys/mman.h>

#include "manage_processes.h"
#include "manage_memory.h"
#include "Shared/shared.h"

static int last_checked_seconds = 0;
static int signal_is_on = 0;

/// We need to figure it out!
int check_signal(){
    return 0;
}

_Noreturn int run_daemon(){

    printf("Daemon initialized\n");

    int initialization_error = initialize_processes();
    if(initialization_error!=0){
        printf("Processed with error %d\n", initialization_error);
        return 0;
    }

    #if DEBUG
        signal_is_on = 1;
    #endif

    while (1) {
        char request[512 + 1];

        //signal_is_on = check_signal();
        /// Somehow catch the signal here
        check_signal();

        if(signal_is_on){

            /// This is just an example
            struct signal_details signal;
            signal.type = ADD;
            signal.priority = MEDIUM;
            signal.path = "/Users/dariusbuhai/Desktop/Programs";

            int error = process_signal(signal);
            if(error)
                printf("Processed with error %d\n", error);
            fflush(stdout);
            signal_is_on = 0;
        }

        int current_time_seconds = time(NULL);

        if (current_time_seconds - last_checked_seconds > CHECK_IN_INTERVAL) {
            /// Any periodic routines here
            printf("Periodic checks \n");
            check_processes();
            last_checked_seconds = current_time_seconds;
        }
    }
}