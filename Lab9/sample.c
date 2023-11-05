
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

void mapHeap(void* address);
void mapStack();
void mapInitialized();
void mapUninitialized();
void SIGSEGV_handler(int sig);

int i = 0;
int j;

int main() {
    signal(SIGSEGV, SIGSEGV_handler);
    
    //mapStack();
    //mapHeap(0);
    //mapInitialized();
    //mapUninitialized();

    printf("Unin: %p\n", &j);
    printf("In: %p\n", &i);
    
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

void mapInitialized(){
    static int initialized = 0;
    printf("Initialized Address: %p\n", &initialized);

    mapInitialized();
}

void mapUninitialized(){
    static int uninitialized;
    printf("Uninitialized Address: %p\n", &uninitialized);

    mapUninitialized();
}

void SIGSEGV_handler(int sig){
    perror("Invalid Storage Access: \n");
    exit(1);
}