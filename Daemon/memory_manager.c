#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include "../Shared/shared.h"
#include "memory_manager.h"

static int shm_fd_processes, shm_fd_counter, shm_fd_task_details;

int create_shm_memory(char shm_name[], int *shm_fd, int size){
    *shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if((*shm_fd)<0){
        #ifdef SHOW_ERRORS
            perror(NULL);
        #endif
        return errno;
    }
    if(ftruncate(*shm_fd, getpagesize()*size)==-1){
        #ifdef SHOW_ERRORS
            perror(NULL);
        #endif
        shm_unlink(shm_name);
        return errno;
    }
    return 0;
}

void* get_shm_ptr(char name[], int shm_fd, int offset, int len){
    void* shm_ptr = mmap(0, len, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, getpagesize()*offset);
    if(shm_ptr == MAP_FAILED){
        #ifdef SHOW_ERRORS
            perror(NULL);
        #endif
        shm_unlink(name);
        return NULL;
    }
    return shm_ptr;
}

void close_shm_ptr(void *shm_ptr, int len) {
    int ret = munmap(shm_ptr, len);
    if (ret == -1) {
      perror("munmap failed");
      return;
    }
}

int initialize_processes(){

    create_shm_memory("process_counter", &shm_fd_counter, 1);
    create_shm_memory("task_details", &shm_fd_task_details, sizeof(int));

    int* process_counter = get_process_counter();
    if(process_counter==NULL)
        return errno;
    *process_counter = 0;
    return 0;
}

int* get_process_counter(){
    void* process_counter = get_shm_ptr("process_counter", shm_fd_counter, 0, sizeof(int));
    return process_counter;
}

int* get_task_details() {
    int* task_details = get_shm_ptr("task_details", shm_fd_task_details, 0, sizeof(int));
    return task_details;
}
