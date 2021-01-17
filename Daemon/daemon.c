#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <syslog.h>

#include "daemon_flow.h"
#include "../Shared/shared.h"

int main(){

    int error = run_daemon();
    if(error)
        printf("Failed to start daemon, error code: %d\n", error);

    return EXIT_SUCCESS;
}
