#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "signal_manager.h"
#include "Shared/shared.h"

static struct signal_details* current_signal = NULL;

/// Save pid to file
void save_daemon_pid(const pid_t pid){
    char* data = malloc(sizeof (char)*MAX_PID_SIZE);
    char *complete_pid_path = get_current_path();
    strcat(complete_pid_path, PID_PATH);

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

    struct signal_details* incoming_signal = malloc(sizeof (struct signal_details));
    sscanf(data, "TYPE %d", &incoming_signal->type);
    if(incoming_signal->type==ADD){
        incoming_signal->path = malloc(sizeof(char)*MAX_FILE_PATH_SIZE);
        sscanf(data, "TYPE %d\nPRIORITY %d\nPATH %s\nPPID %d", &incoming_signal->type, &incoming_signal->priority, incoming_signal->path, &incoming_signal->ppid);
    }
    if(incoming_signal->type==SUSPEND || incoming_signal->type==RESUME || incoming_signal->type==KILL){
        incoming_signal->path = malloc(sizeof(char)*MAX_FILE_PATH_SIZE);
        sscanf(data, "TYPE %d\nPID %d\nPPID %d", &incoming_signal->type, &incoming_signal->pid, &incoming_signal->ppid);
    }
    return incoming_signal;
}

/// Handle incoming signal
void handle_incoming_signal(int signo){
    printf("Received Signal %d on USR1\n", signo);
    current_signal = read_daemon_instruction();
}

int initialize_signals(const pid_t pid){
    save_daemon_pid(pid);
    signal(SIGINT, handle_incoming_signal);
    return 0;
}

struct signal_details* get_current_signal(){
    return current_signal;
}

void reset_current_signal(){
    current_signal = NULL;
}

int send_signal(pid_t pid){
    kill(pid, SIGINT);
    #ifdef DEBUG
        printf("Signal send to pid: %d\n", pid);
    #endif
    return 0;
}