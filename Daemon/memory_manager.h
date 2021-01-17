#ifndef DISKANALYZER_MEMORY_MANAGER_H
#define DISKANALYZER_MEMORY_MANAGER_H

int initialize_processes();
int* get_process_counter();
int* get_task_details();
struct process_details* get_process_details(int);
void close_shm_ptr(void*, int);
// TODO teardown function

#endif //DISKANALYZER_MEMORY_MANAGER_H
