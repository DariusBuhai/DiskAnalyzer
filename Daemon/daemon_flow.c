#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#include "process_manager.h"
#include "memory_manager.h"
#include "signal_manager.h"

static int last_checked_seconds = 0;

int run_daemon(){

    puts("1");
    init();
    puts("2");
    printf("Daemon initialized. Pid: %d\n", getpid());

    int initialization_error = initialize_processes();

    if(initialization_error!=0){
        printf("Processed with error %d\n", initialization_error);
        return initialization_error;
    }

    while (1) {
        update_ids();
        if(get_current_signal()!=NULL){
            int error = process_signal(*get_current_signal());
            if(error)
                printf("Processed with error %d\n", error);
            fflush(stdout);
            reset_current_signal();
        }

        take_new_task();
        sleep(1);
        /*  */
        /* int current_time_seconds = time(NULL); */
        /*  */
        /* if (current_time_seconds - last_checked_seconds > CHECK_IN_INTERVAL) { */
        /*     check_processes(); */
        /*     last_checked_seconds = current_time_seconds; */
        /* } */
    }

    return 0;
}
