#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int main() {

    long maxPageSize = sysconf(_SC_PAGESIZE);
    printf("Max page size: %ld\n", maxPageSize);

    return 0;
}