#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>

#include "manage_processes.h"
#include "manage_memory.h"
#include "Shared/shared.h"
#include "Worker/analyzer.h"

void check_processes(){
    int process_counter = *get_process_counter();
    /// Update processes
    for(int i=0;i<process_counter;++i)
        if(get_process_details(i)->status!=DONE){
            int status;
            pid_t return_pid = waitpid(get_process_details(i)->pid, &status, WNOHANG);
            if(return_pid==-1){
                perror(NULL);
                printf("Process error: %d\n", errno);
            }else if(return_pid==get_process_details(i)->pid)
                get_process_details(i)->status = DONE;
        }

    /// Example
    if(process_counter>0){
        struct process_details* current = get_process_details(process_counter-1);
        printf("Analysing path: %s. Found %d files, Done: %d, Perc: %d%%. \n",current->path, current->total_tasks, current->done_tasks, (current->done_tasks/current->total_tasks)*100);
    }

    /// Get highest priority at the moment
    int highest_priority = LOW;
    for(int i=0;i<process_counter;++i)
        if(get_process_details(i)->status==RUNNING && get_process_details(i)->priority>highest_priority)
            highest_priority = get_process_details(i)->priority;
    for(int i=0;i<process_counter;++i)
        if(get_process_details(i)->priority<highest_priority && get_process_details(i)->status==RUNNING){
            get_process_details(i)->status = PAUSED;
            kill(get_process_details(i)->pid, SIGSTOP);
        }
        else if(get_process_details(i)->status==PAUSED)
            kill(get_process_details(i)->pid, SIGCONT);
}

int process_signal(struct signal_details signal){
    if(signal.type==ADD){
        pid_t pid = fork();
        if(pid<0)
            return errno;
        if(pid==0){
            /// Manage child process
            struct process_details* process = get_process_details((*get_process_counter())++);

            process->pid = getpid();
            process->priority = signal.priority;
            process->path = signal.path;

            printf("Process with id: %d has just started\n", process->pid);

            analyze_dir(signal.path, process);
        }
    }
    /// Pause process id
    if(signal.type==PAUSE){
        /// Determine the process running with that pid somehow
        //process.status = FORCE_PAUSED;
        kill(signal.pid, SIGSTOP);
    }
    /// Continue process with id
    if(signal.type==RESUME){
        /// Determine the process running with that pid somehow
        //process.status = RUNNING;
        kill(signal.pid, SIGCONT);
    }
    /// Kill process
    if(signal.type==KILL){
        /// Determine the process running with that pid somehow
        //process.status = KILLED;
        kill(signal.pid, SIGTERM);
    }

    /// Important! Check processes
    check_processes();
    return 0;
}