
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>

void mapHeap(int position);
void mapStack(int position);
void SIGSEGV_handler(int sig);

// (Un)Initialized variables can either go here or be static.
int firstUninitialized;
int secondUninitialized;
int thirdUninitialized;

int firstInitialized = 1;
int secondInitialized = 2;
int thirdInitialized = 3;

int main() {
    signal(SIGSEGV, SIGSEGV_handler);

    printf("------Stack Information------\n\n");

    printf("\nNegative Growth Demonstration:\n");
    mapStack(5);

    printf("\n------Heap Information------\n\n");
    
    printf("\nPositive Growth Demonstration:\n");
    mapHeap(5);

    printf("\n------Uninitialized Data Information------\n\n");
    printf("Address: %p\n", &firstUninitialized);
    printf("Address: %p\n", &secondUninitialized);
    printf("Address: %p\n", &thirdUninitialized);

    printf("\n------Initialized Data Information------\n\n");
    printf("Address: %p\n", &firstInitialized);
    printf("Address: %p\n", &secondInitialized);
    printf("Address: %p\n", &thirdInitialized);

    printf("\n------Program Text Information------\n\n");
    printf("Coming Soon\n");

    return 0;
}

void mapStack(int position) {
    auto int temp = 0;
    printf("Stack Address: %p\n", &temp);

    if(position > 0){
        mapStack(position - 1);
    }else{
        printf("\n");
    } 
}

void mapHeap(int position) {
    void* stackAddress;

    if ((stackAddress = malloc(sizeof(void*))) != NULL) {
        printf("Heap Address: %p\n", stackAddress);

    }
    else {
        printf("Error after %p\n", stackAddress);
        return;
    }

    if(position > 0){
        mapHeap(position - 1);
    }else{
        free(stackAddress);
    }
}

void SIGSEGV_handler(int sig){
    perror("Invalid Storage Access: \n");
    exit(1);
}