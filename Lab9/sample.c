
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/resource.h>

void mapHeap(int position);
void mapStack(int position);
void mapInitialized();
void mapUninitialized();
void SIGSEGV_handler(int sig);

// (Un)Initialized variables can either go here or be static.

struct rlimit stackSize;

int main() {
    signal(SIGSEGV, SIGSEGV_handler);

    printf("------Stack Information------\n\n");

    int stackVariable = 0;
    int *stackStartAddress = &stackVariable + 1;

    printf("Start Address: %p\n", stackStartAddress);

    if(getrlimit(RLIMIT_STACK, &stackSize) == -1){
        perror("getrlimit failed");
        exit(1);
    }

    int stackSizeHex = stackSize.rlim_cur / 4;
    int *stackEndAddress = &stackVariable + 1 - stackSizeHex;

    printf("End Address: %p\n", stackEndAddress);

    printf("\nNegative Growth Demonstration:\n");
    mapStack(5);

    printf("\n------Heap Information------\n\n");
    
    int *heapVariable = malloc(sizeof(int));

    printf("Top of the Heap (3-Bytes Off): %p\n", stackEndAddress - 1);
    printf("Bottom of the Heap: %p\n", heapVariable - 1);

    printf("\nPositive Growth Demonstration:\n");
    mapHeap(5);

    printf("\n------Uninitialized Data Information------\n\n");
    printf("COMING SOON\n");
    
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

void mapInitialized(){
}

void mapUninitialized(){
}

void SIGSEGV_handler(int sig){
    perror("Invalid Storage Access: \n");
    exit(1);
}