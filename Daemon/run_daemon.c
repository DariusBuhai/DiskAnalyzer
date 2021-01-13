#include <stdio.h>
#include <time.h>
#include <sys/mman.h>

#include "manage_processes.h"
#include "Shared/shared.h"

static int last_checked_seconds = 0;
static int signal_is_on = 0;

/// No idea how
int check_signal(){

    return 0;
}

_Noreturn int run_daemon(){

    printf("Daemon initialized\n");

    initialize_processes();

    #if DEBUG
        signal_is_on = 1;
    #endif

    while (1) {
        char request[512 + 1];

        /// check_signal()
        /// Somehow catch the signal here
        if(signal_is_on){
            int error = process_requests(request);
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