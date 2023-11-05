
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

int firstInitialized = 0;
int secondInitialized = 0;
int thirdInitialized = 0;

int firstUninitialized;
int secondUninitialized;
int thirdUninitialized;
int fourthIn = 0;


int fourthUnin;

int main() {
    signal(SIGSEGV, SIGSEGV_handler);
    
    //mapStack();
    //mapHeap(0);

    
    printf("Uninitialized Address: %p\n", &fourthUnin);
    printf("Uninitialized Address: %p\n", &thirdUninitialized);
    printf("Uninitialized Address: %p\n", &secondUninitialized);
    printf("Uninitialized Address: %p\n", &firstUninitialized);

    printf("Initialized Address: %p\n", &fourthIn);
    printf("Initialized Address: %p\n", &thirdInitialized);
    printf("Initialized Address: %p\n", &secondInitialized);
    printf("Initialized Address: %p\n", &firstInitialized);
    
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
}

void mapUninitialized(){
}

void SIGSEGV_handler(int sig){
    perror("Invalid Storage Access: \n");
    exit(1);
}