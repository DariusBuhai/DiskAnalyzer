#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "manage_processes.h"
#include "manage_memory.h"
#include "Shared/shared.h"

static int last_checked_seconds = 0;
static int signal_is_on = 0;

/// Save current pid
void save_to_file(char* file_location, char data[]){
    struct stat buffer;
    if(stat(file_location, &buffer)>=0)
        remove(file_location);
    int tf = open(file_location, O_CREAT | O_WRONLY, S_IRWXU);
    if(write(tf, data, sizeof (char)*strlen(data)) <= 0)
        perror(NULL);
    close(tf);
}

void save_daemon_pid(const pid_t pid){
    char data[100] = {0};
    sprintf(data, "%d", pid);
    save_to_file("../Temp/daemon.pid", data);
}

/// Handle received signals
void sig_handler_usr1(int signo){
    printf("Received Signal %d on USR1\n", signo);
}

void sig_handler_usr2(int signo){
    printf("Received Signal %d on USR2\n", signo);
}

int check_signal(){
    signal(SIGUSR1, sig_handler_usr1);
    signal(SIGUSR2, sig_handler_usr2);
    return 0;
}

_Noreturn int run_daemon(){

    printf("Daemon initialized. Pid: %d\n", getpid());
    save_daemon_pid(getpid());

    int initialization_error = initialize_processes();
    if(initialization_error!=0){
        printf("Processed with error %d\n", initialization_error);
        return 0;
    }

    #if DEBUG
        signal_is_on = 0;
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
            /// Periodic routines
            check_processes();
            last_checked_seconds = current_time_seconds;
        }
    }
}