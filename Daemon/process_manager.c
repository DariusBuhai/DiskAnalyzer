#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>

#include "process_manager.h"
#include "memory_manager.h"
#include "signal_manager.h"
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
        if(current->status==RUNNING)
            printf("Analysing path: %s. Found %d files, Done: %d. \n",current->path, current->total_tasks, current->done_tasks);
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

            /// Send result back to client, verify eligibility here!
            char* output = malloc(sizeof(char)*1024);
            sprintf(output, "Created analysis task with ID `%d` for `%s` and priority `%s`", process->pid, process->path, get_literal_priority(process->priority));
            write_daemon_output(output);
            send_signal(signal.ppid);

            /// Call function
            analyze_dir(signal.path, process);
        }
    }

    if(signal.type==SUSPEND || signal.type==RESUME || signal.type==KILL){
        /// Determine the process running with that pid
        struct process_details* process = NULL;
        for(int i=0;i<*get_process_counter();++i)
            if(get_process_details(i)->pid==signal.pid)
                process = get_process_details(i);

        char* output = malloc(sizeof(char)*1024);
        if(process!=NULL){
            if(signal.type==SUSPEND){
                kill(signal.pid, SIGSTOP);
                process->status = FORCE_PAUSED;
                sprintf(output, "Task with ID `%d` suspended", signal.pid);
            }else if(signal.type==RESUME){
                kill(signal.pid, SIGCONT);
                process->status = RUNNING;
                sprintf(output, "Task with ID `%d` resumed", signal.pid);
            }else{
                kill(signal.pid, SIGTERM);
                process->status = KILLED;
                sprintf(output, "Removed analysis task with ID `%d` for `%s`", signal.pid, process->path);
            }
        }else
            sprintf(output, "Cannot find task with ID `%d`", signal.pid);
        write_daemon_output(output);
        send_signal(signal.ppid);
    }

    /// Check processes
    check_processes();
    return 0;
}