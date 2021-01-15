#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "memory_manager.h"
#include "Shared/shared.h"

static int shm_fd_processes, shm_fd_counter;

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

void* get_shm_ptr(char name[], int shm_fd, int offset){
    void* shm_ptr = mmap(0, getpagesize(), PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, getpagesize()*offset);
    if(shm_ptr == MAP_FAILED){
        #ifdef SHOW_ERRORS
            perror(NULL);
        #endif
        shm_unlink(name);
        return NULL;
    }
    return shm_ptr;
}

int initialize_processes(){
    create_shm_memory("processes", &shm_fd_processes, ALLOWED_PROCESSES);
    create_shm_memory("process_counter", &shm_fd_counter, 1);

    for(int i=0;i<ALLOWED_PROCESSES;++i){
        struct process_details* process = get_process_details(i);
        if(process==NULL)
            return errno;
    }
    int* process_counter = get_process_counter();
    if(process_counter==NULL)
        return errno;
    *process_counter = 0;
    return 0;
}

struct process_details* get_process_details(int id){
    void* process = get_shm_ptr("processes", shm_fd_processes, id);
    if(process==NULL)
        return NULL;
    return process;
}

int* get_process_counter(){
    void* process_counter = get_shm_ptr("process_counter", shm_fd_counter, 0);
    if(process_counter==NULL)
        return NULL;
    return process_counter;
}
