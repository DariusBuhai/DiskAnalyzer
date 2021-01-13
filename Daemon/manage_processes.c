#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include "manage_processes.h"
#include "Shared/shared.h"
#include "Worker/storage.h"

struct process_details* active_processes;
int no_of_processes = 0;

void initialize_processes(){
    active_processes = malloc(sizeof(struct process_details)*ALLOWED_PROCESSES);
    no_of_processes = 0;
}

void check_processes(){
    /// Update processes
    for(int i=0;i<no_of_processes;++i){
        int status;
        pid_t return_pid = waitpid(active_processes[i].pid, &status, WNOHANG);
        if(return_pid==-1){
            /// error here
        }else if(return_pid==0){
            /// still running
        }else if(return_pid==active_processes[i].pid){
            active_processes[i].status = DONE;
        }
    }
    /// Get highest priority at the moment
    int highest_priority = LOW;
    for(int i=0;i<no_of_processes;++i)
        if(active_processes->status==RUNNING && active_processes[i].priority>highest_priority)
            highest_priority = active_processes[i].priority;
    for(int i=0;i<no_of_processes;++i)
        if(active_processes[i].priority<highest_priority && active_processes[i].status==RUNNING){
            active_processes[i].status = PAUSED;
            kill(active_processes[i].pid, SIGSTOP);
        }
        else if(active_processes[i].status==PAUSED)
            kill(active_processes[i].pid, SIGCONT);

}

int process_requests(char* request){
    int signal_type = 1;
    if(signal_type==1){
        char *file = "/Users/dariusbuhai/Desktop/Programs";
        pid_t pid = fork();
        if(pid<0)
            return errno;
        if(pid==0){
            /// Manage child process
            active_processes[no_of_processes++].pid = getpid();
            active_processes->priority = LOW;
            printf("Process with id: %d has just started\n", getpid());
            getpid();
            analyze_dir(file);
        }
    }
    struct process_details process_id;
    /// Cannot yet determine the process_id, create a random one
    if(DEBUG && no_of_processes>0)
        process_id = active_processes[no_of_processes-1];
    /// Pause process id
    if(signal_type==2){
        process_id.status = FORCE_PAUSED;
        kill(process_id.pid, SIGSTOP);
    }
    /// Continue process with id
    if(signal_type==2){
        process_id.status = RUNNING;
        kill(process_id.pid, SIGCONT);
    }
    /// Kill process
    if(signal_type==2){
        process_id.status = KILLED;
        kill(process_id.pid, SIGTERM);
    }

    /// Important! Check all processes priorities
    check_processes();
    return 0;
}