#include <stdio.h>
#include <time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

#include "signal_manager.h"
#include "Shared/shared.h"
#include "process_manager.h"

static struct signal_details* current_signal = NULL;

void write_to_file(char* file_location, char data[]){
    struct stat buffer;
    if(stat(file_location, &buffer)>=0)
        remove(file_location);
    int tf = open(file_location, O_CREAT | O_WRONLY, S_IRWXU);
    if(write(tf, data, sizeof (char)*strlen(data)) <= 0)
        perror(NULL);
    close(tf);
}

char* read_from_file(char* file_location){
    struct stat buffer;
    if(stat(file_location, &buffer)<0){
        printf("No file available!\n");
        return 0;
    }
    FILE* fp = fopen(file_location, "r");
    char* data = malloc(sizeof(char)*buffer.st_size);

    fread(data, buffer.st_size, 1, fp);
    return data;
}

/// Save pid to file
void save_daemon_pid(const pid_t pid){
    char* data;
    sprintf(data, "%d", pid);
    write_to_file("../TempData/daemon.pid", data);
}

/// Handle received signals
void handle_incoming_signal(int signo){
    printf("Received Signal %d on USR1\n", signo);
    char* data = read_from_file("../TempData/daemon_instruction.txt");
    current_signal = malloc(sizeof (struct signal_details));
    sscanf(data, "TYPE %d", &current_signal->type);
    if(current_signal->type==ADD){
        current_signal->path = malloc(sizeof(char)*MAX_FILE_PATH_SIZE);
        sscanf(data, "TYPE %d\nPRIORITY %d\nPATH %s\nPPID %d", &current_signal->type, &current_signal->priority, current_signal->path, &current_signal->ppid);
        return;
    }
    if(current_signal->type==SUSPEND || current_signal->type==RESUME || current_signal->type==KILL){
        current_signal->path = malloc(sizeof(char)*MAX_FILE_PATH_SIZE);
        sscanf(data, "TYPE %d\nPID %d\nPPID %d", &current_signal->type, &current_signal->pid, &current_signal->ppid);
        return;
    }

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