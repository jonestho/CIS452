
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void mapHeap(void* address);
void mapStack();
void SIGSEGV_handler(int sig);

int main() {
    signal(SIGSEGV, SIGSEGV_handler);
    mapStack();
    mapHeap(0);

    return 0;
}


void mapStack() {
    auto int temp = 0;
    printf("Stack Address: %p\n", &temp);
    mapStack();
}

void mapHeap(void* address) {
    void* stackAddress;

    if ((stackAddress = malloc(sizeof(void*))) != NULL) {
        printf("Heap Address: %p\n", stackAddress);

    }
    else {
        printf("Error after %p\n", address);
        return;
    }

    mapHeap(stackAddress);
    free(stackAddress);
}

