#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

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


/// We need to figure it out!
void signal_handler(int sig) {
    char s1[] = "Hello world\n";
    write(STDOUT_FILENO, s1, sizeof(s1));
    signal(SIGUSR1, signal_handler);
}

pid_t get_daemon_pid(){
    char *file_location = "Temp/daemon.pid";
    struct stat buffer;
    if(stat(file_location, &buffer)<0){
        printf("No pid available!\n");
        return 0;
    }
    FILE* fp = fopen(file_location, "r");
    char data[buffer.st_size];
    fscanf(fp, "%s", data);
    return atoi(data);
}

int send_signal(){
    signal(SIGUSR1, signal_handler);
    pid_t pid = get_daemon_pid();
    if(pid==0)
        return 0;
    kill(pid, SIGUSR1);
    printf("Signal send to pid: %d\n", pid);
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
