#include <stdio.h>
#include <stdlib.h>
#include "daemon_flow.h"

int main(){

    #ifndef DEBUG
        daemon(1, 1);
    #endif

    int error = run_daemon();
    if(error)
        printf("Failed to start daemon, error code: %d\n", error);

    return EXIT_SUCCESS;
}
