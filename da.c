#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>

char *get_literal_priority(int priority){
    if(priority==1)
        return "low";
    if(priority==2)
        return "normal";
    return "high";
}

int is_option(char* option, char* str1, char *str2){
    return strcmp(option, str1)==0 || strcmp(option, str2)==1;
}

/// No idea how
int send_signal(){
    return 0;
}

int main(int argc, char **argv){
    if(argc==1)
        return 0;

    // Add
    if(is_option(argv[1], "-a", "--add")){
        if(argc<3)
            return -1;
        int priority = 1;
        char* path = argv[2];
        // Specify priority
        if(argc==5 && is_option(argv[3], "-p", "--priority")){
            priority = atoi(argv[4]);
            if(priority<1 || priority>3){
                printf("Priority should have one of the values: 1-low, 2-normal, 3-high\n");
                priority = 1;
            }
        }
        // Execute
        send_signal();
        // Print result
        printf("Created analysis task with ID `%d` for `%s` and priority `%s`.\n",2, path, get_literal_priority(priority));
        return 0;
    }
    // Suspend
    if(is_option(argv[1], "-S", "--suspend")){
        if(argc<3)
            return -1;
        int taskId = atoi(argv[2]);
        printf("Task with ID `%d` suspended.\n", taskId);
    }
    // Resume
    if(is_option(argv[1], "-R", "--resume")){
        if(argc<3)
            return -1;
        int taskId = atoi(argv[2]);
        printf("Task with ID `%d` resumed.\n", taskId);
    }
    // Remove
    if(is_option(argv[1], "-r", "--remove")){
        if(argc<3)
            return -1;
        int taskId = atoi(argv[2]);
        char* status = "done", *path = "/home/user";
        printf("Removed analysis task with ID `%d`, status `%s` for `%s`\n", taskId, status, path);
    }
    // Info
    if(is_option(argv[1], "-i", "--info")){
        if(argc<3)
            return -1;
        int taskId = atoi(argv[2]);
        char* status = "done";
        printf("The analysis of task with ID `%d` has the status `%s`\n", taskId, status);
    }
    // List
    if(is_option(argv[1], "-l", "--list")){
        //char* status = "done";
        printf("ID  PRI  Path  Done  Status  Details\n");
    }
    // Analysis report for tasks that are "done"
    if(is_option(argv[1], "-p", "--print")){
        if(argc<3)
            return -1;
        //int taskId = atoi(argv[2]);
        printf("Path  Usage  Size  Amount\n");
    }

    return 0;
}
