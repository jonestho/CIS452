#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/resource.h>

int main() {
    struct rlimit maxOpenFiles;

    if (getrlimit(RLIMIT_NOFILE, &maxOpenFiles) == -1) {
        perror("getrlimit failed");
        exit(1);
    }

    printf("(Hard) Max Open Files: %ld\n", maxOpenFiles.rlim_max);
    printf("(soft) Max Open Files: %ld\n", maxOpenFiles.rlim_cur);

    return 0;
}