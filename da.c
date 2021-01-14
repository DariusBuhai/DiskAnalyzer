#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ADD 1
#define SUSPEND 2
#define RESUME 3
#define KILL 4

char* get_literal_priority(int priority){
    if(priority==1)
        return "low";
    if(priority==2)
        return "normal";
    return "high";
}

int is_option(char* option, char* str1, char *str2){
    return strcmp(option, str1)==0 || strcmp(option, str2)==1;
}

char* itoa(int x,int size){
    char* data = malloc(sizeof(char)*size);
    sprintf(data, "%d", x);
    return data;
}

void write_to_file(char* file_location, char data[]){
    struct stat buffer;
    if(stat(file_location, &buffer)>=0)
        remove(file_location);
    int tf = open(file_location, O_CREAT | O_WRONLY, S_IRWXU);
    if(write(tf, data, sizeof (char)*strlen(data)) <= 0)
        perror(NULL);
    close(tf);
}

pid_t get_daemon_pid(){
    char *file_location = "TempData/daemon.pid";
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

int initialize_signal(){
    signal(SIGUSR2, NULL);
    return 0;
}

int send_signal(){
    pid_t pid = get_daemon_pid();
    if(pid==0)
        return 0;
    kill(pid, SIGINT);
    printf("Signal send to pid: %d\n", pid);
    return 0;
}

int main(int argc, char **argv){
    if(argc==1)
        return 0;
    initialize_signal();
    /// Add
    if(is_option(argv[1], "-a", "--add")){
        if(argc<3)
            return -1;
        int priority = 1;
        char* path = argv[2];
        /// Specified priority
        if(argc==5 && is_option(argv[3], "-p", "--priority")){
            priority = atoi(argv[4]);
            if(priority<1 || priority>3){
                printf("Priority should have one of the values: 1-low, 2-normal, 3-high\n");
                priority = 1;
            }
        }
        /// Send signal instruction
        char* instructions = malloc(sizeof(char)*1024);
        sprintf(instructions, "TYPE %d\nPRIORITY %d\nPATH %s\nPPID %d", ADD, priority, path, getpid());
        write_to_file("TempData/daemon_instruction.txt", instructions);
        send_signal();
        /// Receive signal
        /// Print result
        printf("Created analysis task with ID `%d` for `%s` and priority `%s`.\n",2, path, get_literal_priority(priority));
        return 0;
    }
    /// Suspend
    if(is_option(argv[1], "-S", "--suspend")){
        if(argc<3)
            return -1;
        int pid = atoi(argv[2]);
        /// Send signal instruction
        char* instructions = malloc(sizeof(char)*1024);
        sprintf(instructions, "TYPE %d\nPID %d\nPPID %d", SUSPEND, pid, getpid());
        write_to_file("TempData/daemon_instruction.txt", instructions);
        send_signal();
        /// Receive signal
        /// Print result
        printf("Task with ID `%d` suspended.\n", pid);
    }
    /// Resume
    if(is_option(argv[1], "-R", "--resume")){
        if(argc<3)
            return -1;
        int pid = atoi(argv[2]);
        /// Send signal instruction
        char* instructions = malloc(sizeof(char)*1024);
        sprintf(instructions, "TYPE %d\nPID %d\nPPID %d", RESUME, pid, getpid());
        write_to_file("TempData/daemon_instruction.txt", instructions);
        send_signal();
        /// Receive signal
        /// Print result
        printf("Task with ID `%d` resumed.\n", pid);
    }
    /// Kill
    if(is_option(argv[1], "-r", "--remove")){
        if(argc<3)
            return -1;
        int pid = atoi(argv[2]);
        char* status = "done", *path = "/home/user";
        /// Send signal instruction
        char* instructions = malloc(sizeof(char)*1024);
        sprintf(instructions, "TYPE %d\nPID %d\nPPID %d", KILL, pid, getpid());
        write_to_file("TempData/daemon_instruction.txt", instructions);
        send_signal();
        /// Receive signal
        /// Print result
        printf("Removed analysis task with ID `%d`, status `%s` for `%s`\n", pid, status, path);
    }
    /// Info
    if(is_option(argv[1], "-i", "--info")){
        if(argc<3)
            return -1;
        int taskId = atoi(argv[2]);
        char* status = "done";
        printf("The analysis of task with ID `%d` has the status `%s`\n", taskId, status);
    }
    /// List
    if(is_option(argv[1], "-l", "--list")){
        //char* status = "done";
        printf("ID  PRI  Path  Done  Status  Details\n");
    }
    /// Analysis report for tasks that are "done"
    if(is_option(argv[1], "-p", "--print")){
        if(argc<3)
            return -1;
        //int taskId = atoi(argv[2]);
        printf("Path  Usage  Size  Amount\n");
    }

    return 0;
}
