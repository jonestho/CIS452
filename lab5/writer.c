#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>

void detach();
void removeSegment();
void attach();
void quitSignalHandler(int sig);
void getUserInput(char* input);

int shmId;
char* sharedMemoryPtr;

#define SM_SIZE 500

int main(int argc, char** argv) {
    signal(SIGINT, quitSignalHandler);

    key_t key = ftok("writer.c", 7);

    if (key < 0) {
        perror("Unable to get key\n");
        exit(1);
    }

    uint8_t *readFlag1, *readFlag2;
    char* stringStart;
    char* userInput = (char*)malloc(sizeof(char) * SM_SIZE - 3);

    // Creates the shared memory segment
    if ((shmId = shmget(key, SM_SIZE, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror("Unable to get shared memory\n");
        exit(1);
    }
    attach();

    /*
        Sets pointers to where they need to point
    
        |WRITE_FLAG | READ_FLAG1 | READ_FLAG2 | STRING_START....|
    
    */
    readFlag1 = (uint8_t*)sharedMemoryPtr + 1;
    readFlag2 = (uint8_t*)sharedMemoryPtr + 2;
    stringStart = (uint8_t*)sharedMemoryPtr + 3; // Start of string

    // Set initial values
    *sharedMemoryPtr = 0; // Write flag
    *readFlag1 = 0; // Read flag 1
    *readFlag2 = 0; // Read flag 2

    while (1) {
        getUserInput(userInput);
        
        // Write to shared memory
        memcpy(stringStart, userInput, strlen(userInput));
        *sharedMemoryPtr = 1;

        while (*readFlag1 != 1 || *readFlag2 != 1);

        *readFlag1 = 0;
        *readFlag2 = 0;

        memset(stringStart, 0, strlen(userInput));
    }

    
    printf("Value a: %p\t Value b: %p\n", (void*)sharedMemoryPtr, (void*)sharedMemoryPtr + SM_SIZE);
    printf("ID: %d\n", shmId);

    return 0;
}


void detach() {
    if (shmdt(sharedMemoryPtr) < 0) {
        perror("Unable to detach\n");
        exit(1);
    }
}

void removeSegment() {
    if (shmctl(shmId, IPC_RMID, 0) < 0) {
        perror("Unable to deallocate\n");
        exit(1);
    }
}

void attach() {
    if ((sharedMemoryPtr = shmat(shmId, 0, 0)) == (void*)-1) {
        perror("Unable to attach\n");
        exit(1);
    }
}

void quitSignalHandler(int sig) {
    *sharedMemoryPtr = 2;
    sleep(1);
    detach();
    removeSegment();

    printf("\n");
    exit(0);
}

void getUserInput(char* input) {
    printf("Enter a string: ");
    fgets(input, SM_SIZE - 3, stdin);
}