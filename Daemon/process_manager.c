#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "process_manager.h"
#include "memory_manager.h"
#include "signal_manager.h"
#include "../Worker/analyzer.h"

int task_cnt = 0;
struct task_details tasks[1024];

void update_ids() {
  // update task statuses according to the changes made by workers
  int *details = get_task_details();
  for (int i = 1; i <= task_cnt; ++ i) {
    if (details[i] != tasks[i].status) {
      tasks[i].status = details[i];
    }
  }
}

void take_new_task() {
    int* process_counter = get_process_counter();
    if (*process_counter == ALLOWED_PROCESSES) {
      return;
    }
    close_shm_ptr(process_counter, sizeof(*process_counter));

    int next_task_id = -1;
    // look for a pending task and start a process for it
    for (int i = 1; i <= task_cnt; ++ i) {
      if (tasks[i].status != T_PENDING) continue;
      if (next_task_id == -1 || tasks[i].priority > tasks[next_task_id].priority) {
        next_task_id = i;
      }
    }

    // no new task
    if (next_task_id == -1) {
      return;
    }

    puts("found new task");
    // found new task
    pid_t pid = fork();

    if (pid < 0) {
      perror("couldn't fork child");
      return;
    }

    if (pid == 0) {
      int *details = get_task_details();
      int* process_counter = get_process_counter();
      details[next_task_id] = T_IN_PROGRESS;

      *process_counter += 1;
      printf("%s, %d\n", tasks[next_task_id].path, next_task_id);
      analyze(tasks[next_task_id].path, next_task_id);
      *process_counter -= 1;

      details[next_task_id] = T_DONE;
      close_shm_ptr(details, sizeof(*details) * getpagesize());
      close_shm_ptr(process_counter, sizeof(*process_counter));
      exit(0);
    }
    else {
      tasks[next_task_id].status = T_IN_PROGRESS;
      tasks[next_task_id].worker_pid = pid;
    }
}

int process_signal(struct signal_details signal){
    // adding a new task in the task list

    if (signal.type == ADD) {
        ++ task_cnt;

        // add task to list
        tasks[task_cnt].task_id = task_cnt;
        strcpy(tasks[task_cnt].path, signal.path);
        tasks[task_cnt].status = T_PENDING;
        tasks[task_cnt].priority = signal.priority;
        tasks[task_cnt].worker_pid = -1;

        // update task_status in shm
        int* task_details = get_task_details();
        task_details[task_cnt] =  T_PENDING;
        close_shm_ptr(task_details, sizeof(*task_details) * getpagesize());

        // Send result back to client, verify eligibility here!
        char output[1024];
        sprintf(output, "Created analysis task with ID `%d` for `%s` and priority `%s`",
            task_cnt, tasks[task_cnt].path, get_literal_priority(tasks[task_cnt].priority));
        write_daemon_output(output);
        send_signal(signal.ppid);
        return 0;
    }

    if (signal.pid <= 0 && signal.type != LIST) {
      write_daemon_output("Please input a valid task id.");
      send_signal(signal.ppid);
      return 0;
    }

    // suspending / resuming / killing a worker
    if (signal.type == SUSPEND || signal.type == RESUME || signal.type == KILL){
        // Determine the process running with that pid
        int worker_pid = tasks[signal.pid].worker_pid;

        char output[1024];
        if (worker_pid != -1 && signal.pid <= task_cnt) {
            int* task_details = get_task_details();
            if (signal.type == SUSPEND){
                kill(worker_pid, SIGSTOP);
                task_details[signal.pid] = T_PAUSED;
                sprintf(output, "Task with ID `%d` suspended", signal.pid);
            }
            else if (signal.type == RESUME){
                kill(worker_pid, SIGCONT);
                task_details[signal.pid] = T_IN_PROGRESS;
                sprintf(output, "Task with ID `%d` resumed", signal.pid);
            }
            else {
                kill(worker_pid, SIGTERM);
                task_details[signal.pid] = T_REMOVED;
                sprintf(output, "Removed analysis task with ID `%d` for `%s`",
                    signal.pid, tasks[signal.pid].path);
            }
            close_shm_ptr(task_details, sizeof(*task_details) * getpagesize());
        }
        else {
            sprintf(output, "Task with ID `%d` is not running, or was never created.", signal.pid);
        }

        if (signal.type == KILL) {

            char *analysis_path = get_current_path();
            strcat(analysis_path, ANALYSIS_PATH);
            sprintf(analysis_path, analysis_path, signal.pid);

            char *status_path = get_current_path();
            strcat(status_path, STATUS_PATH);
            sprintf(status_path, status_path, signal.pid);

            remove(analysis_path);
            remove(status_path);
        }

        write_daemon_output(output);
        send_signal(signal.ppid);
        return 0;
    }

    if (signal.type == INFO){
         // Determine the process running with that pid
        char output[1024];
        if (signal.pid <= task_cnt && tasks[signal.pid].status != T_REMOVED) {

            char *status_path = get_current_path();
            strcat(status_path, STATUS_PATH);
            sprintf(status_path, status_path, signal.pid);

          FILE* fd = fopen(status_path, "r");

          int files, dirs, percentage;
          fscanf(fd, "%d%%\n%d files\n%d dirs",
              &percentage, &files, &dirs);
          fclose(fd);

          char pri[] = "***";
          pri[tasks[signal.pid].priority] = '\0';

          sprintf(output, "ID  PRI PATH  DONE  STATUS  DETAILS\n%d  "
              "%s  %s  %d%%  %s  %d files, %d dirs", 
              signal.pid, pri, tasks[signal.pid].path, percentage,
              get_literal_status(tasks[signal.pid].status), files, dirs);
        }
        else {
            sprintf(output, "Task with ID `%d` does not exist.", signal.pid);
        }
        write_daemon_output(output);
        send_signal(signal.ppid);
        return 0;
    }

    if (signal.type == LIST) {
        char output[4096] = "";
        sprintf(output,"ID  PRI  PATH  Done  Status  Details\n");
        int max_len = 4096;

        for(int i = 1; i <= task_cnt; ++ i) {
            if (tasks[i].status == T_REMOVED) continue;

            char *status_path = get_current_path();
            strcat(status_path, STATUS_PATH);
            sprintf(status_path, status_path, i);

            FILE* fd = fopen(status_path, "r");

            int files, dirs, percentage;
            fscanf(fd, "%d%%\n%d files\n%d dirs",
                &percentage, &files, &dirs);
            fclose(fd);

            char pri[] = "***";
            pri[tasks[i].priority] = '\0';

            snprintf(output + strlen(output), max_len - strlen(output), 
                "%d  %s  %s  %d%%  %s  %d files, %d dirs\n", 
                i, pri, tasks[i].path, percentage,
                get_literal_status(tasks[i].status), files, dirs);
        }
        write_daemon_output(output);
        send_signal(signal.ppid);
        return 0;
    }

    if (signal.type == PRINT) {
        char output[2048];
        if (tasks[signal.pid].status == T_DONE) {

            char *analysis_path = get_current_path();
            strcat(analysis_path, ANALYSIS_PATH);
            sprintf(analysis_path, analysis_path, signal.pid);

            FILE* fd = fopen(analysis_path, "r");
        }

        write_daemon_output(output);
        send_signal(signal.ppid);
    }
    return 0;
}
