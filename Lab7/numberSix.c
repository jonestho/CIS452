#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>

int main() {

    long maxPages = sysconf(_SC_PHYS_PAGES);
    printf("Max page size: %ld\n", maxPages);

    return 0;
}