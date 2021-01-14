#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#include "process_manager.h"
#include "memory_manager.h"
#include "signal_manager.h"
#include "Shared/shared.h"

static int last_checked_seconds = 0;

_Noreturn int run_daemon(){

    printf("Daemon initialized. Pid: %d\n", getpid());
    initialize_signals(getpid());

    int initialization_error = initialize_processes();
    if(initialization_error!=0){
        printf("Processed with error %d\n", initialization_error);
        return 0;
    }

    while (1) {

        if(get_current_signal()!=NULL){
            int error = process_signal(*get_current_signal());
            if(error)
                printf("Processed with error %d\n", error);
            fflush(stdout);
            reset_current_signal();
        }

        int current_time_seconds = time(NULL);

        if (current_time_seconds - last_checked_seconds > CHECK_IN_INTERVAL) {
            check_processes();
            last_checked_seconds = current_time_seconds;
        }
    }
}