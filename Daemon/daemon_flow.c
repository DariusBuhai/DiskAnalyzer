#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>

#include "process_manager.h"
#include "memory_manager.h"
#include "signal_manager.h"

int run_daemon(){

    init();
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
    }

    return 0;
}
