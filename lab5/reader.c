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

#define SM_SIZE 500

void detach();
void removeSegment();
void attach();

int shmID;
char *sharedMemoryPtr;

int main(int argc, char **argv)
{
    int readID = atoi(argv[1]); // Signifies if Reader 1 or 2

    uint8_t *readFlag; // Points to the corresponding has-read flag
    char *string;      // beginning of the string

    key_t key = ftok("writer.c", 7);

    if (key == -1)
    {
        perror("Key not found\n");
        exit(1);
    }

    if ((shmID = shmget(key, SM_SIZE, S_IRUSR)) < 0)
    {
        perror("Unable to get shared memory\n");
        exit(1);
    }
    attach();

    readFlag = (uint8_t *)sharedMemoryPtr + readID;
    string = (uint8_t *)sharedMemoryPtr + 3;

    while (1)
    {
        while (*sharedMemoryPtr != 1)
            if (*sharedMemoryPtr == 2) exit(0);
        

        printf("Received from Writer: %s", string);

        *readFlag = 1;
        *sharedMemoryPtr = 0;
    }

    return 0;
}

void detach()
{
    if (shmdt(sharedMemoryPtr) < 0)
    {
        perror("Unable to detach\n");
        exit(1);
    }
}

void removeSegment()
{
    if (shmctl(shmID, IPC_RMID, 0) < 0)
    {
        perror("Unable to deallocate\n");
        exit(1);
    }
}

void attach()
{
    if ((sharedMemoryPtr = shmat(shmID, 0, 0)) == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }
}