#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rlimit maxFileSize;

    if (getrlimit(RLIMIT_FSIZE, &maxFileSize) == -1) {
        perror("getrlimit failed");
        exit(1);
    }

    printf("Max File Size: %ld\n", maxFileSize.rlim_max);
    printf("Current File Size: %ld\n", maxFileSize.rlim_cur);

    return 0;
}