#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/resource.h>

int main(int argc, char **argv){
    long clockResPerSecond = sysconf(_SC_CLK_TCK);
    printf("Total Clock Ticks per Second: %ld\n", clockResPerSecond);

    double clockRes = 1.0 / clockResPerSecond;
    printf("Clock Resolution (sec): %f\n", clockRes);
    printf("Clock Resolution (ms): %f\n", (clockRes * 1000));


    return 0;
}