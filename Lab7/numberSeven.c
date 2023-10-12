#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rlimit maxProcesses;

    getrlimit(RLIMIT_NPROC, &maxProcesses);
    printf("Max Processes: %ld\n", maxProcesses.rlim_max);

    return 0;
}