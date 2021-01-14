#include <stdio.h>
#include <stdlib.h>

#include "Worker/analyzer.h"

#define LL long long

int main(){
    /// test output
    char *file = "/Users/dariusbuhai/Desktop/Programs/C";
    LL total_size = analyze_dir(file);

    printf("Total size: %LfMB, %LfGB\n", bytes_to_xb(total_size, 2), bytes_to_xb(total_size, 3));
}