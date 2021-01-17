#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "signal_manager.h"

static struct signal_details* current_signal = NULL;

/// Save pid to file
void save_daemon_pid(const pid_t pid){
    char *complete_pid_path = get_current_path();
    strcat(complete_pid_path, PID_PATH);
    printf("Saving PID to: %s\n",complete_pid_path );
    char data[MAX_PID_SIZE];
    sprintf(data, "%d", pid);
    write_to_file(complete_pid_path, data);
}

void write_daemon_output(char* data){
    char* complete_output_path = get_current_path();
    strcat(complete_output_path, OUTPUT_PATH);
    write_to_file(complete_output_path, data);
}

struct signal_details* read_daemon_instruction(){
    char* complete_instruction_path = get_current_path();
    strcat(complete_instruction_path, INSTRUCTION_PATH);
    char* data = read_from_file(complete_instruction_path);

    struct signal_details* incoming_signal = malloc(sizeof (*incoming_signal));
    sscanf(data, "TYPE %d", &incoming_signal->type);

    if (incoming_signal->type == ADD) {
        incoming_signal->path = malloc(sizeof(char) * FILENAME_MAX);
        sscanf(data, "TYPE %d\nPRIORITY %d\nPATH %s\nPPID %d",
            &incoming_signal->type, &incoming_signal->priority,
            incoming_signal->path, &incoming_signal->ppid);
    }

    if (incoming_signal->type == SUSPEND || incoming_signal->type == RESUME ||
        incoming_signal->type == KILL || incoming_signal->type == INFO ||
        incoming_signal->type == LIST || incoming_signal->type == PRINT) {

        incoming_signal->path = malloc(sizeof(char)*FILENAME_MAX);
        sscanf(data, "TYPE %d\nPID %d\nPPID %d", &incoming_signal->type,
            &incoming_signal->pid, &incoming_signal->ppid);
    }

    free(data);
    free(complete_instruction_path);
    return incoming_signal;
}

/// Handle incoming signal
void handle_incoming_signal(int signo){
    printf("Received Signal %d on USR1\n", signo);
    if (current_signal) {
      free(current_signal->path);
      free(current_signal);
    }
    current_signal = read_daemon_instruction();
}

void init(){
    save_daemon_pid(getpid());
    signal(SIGUSR1, handle_incoming_signal);
}

struct signal_details* get_current_signal(){
    return current_signal;
}

void reset_current_signal(){
    current_signal = NULL;
}

int send_signal(pid_t pid){
    kill(pid, SIGUSR2);
    #ifdef DEBUG
        printf("Signal send to pid: %d\n", pid);
    #endif
    return 0;
}
