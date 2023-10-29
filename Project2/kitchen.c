// #pragma once
#include "kitchen.h"

int bakersWorking;

int main(int argc, char** argv) {
    pthread_t dispatch;
    void *dispatchResult;

    int dispatchStatus;
    int dispatchJoinResult;


    int sharedMemoryID, *sharedMemoryPtr;
    int semIDs[4]; // pantrySemID, fridgeSemID, utensilSemID, ovenSemID

    char* userInput = (char*) malloc(sizeof(char) * 10);

    // Creating Semaphores
    semIDs[0] = createSemaphore(semIDs[0]); // pantrySemID
    semIDs[1] = createSemaphore(semIDs[1]); // fridgeSemID
    semIDs[2] = createSemaphore(semIDs[2]); // utensilSemID
    semIDs[3] = createSemaphore(semIDs[3]); // ovenSemID

    // Create Shared Memory for Semaphores
    sharedMemoryID = shmget(IPC_PRIVATE, sizeof(semIDs), S_IRUSR | S_IWUSR | IPC_CREAT);

    if(sharedMemoryID < 0){
        perror("shmget: Unable to obtain shared memory\n");
        exit(1);
    }

    sharedMemoryPtr = shmat(sharedMemoryID, 0, 0);

    if (sharedMemoryPtr == (void *)-1)
    {
        perror("shmget: Unable to attach\n");
        exit(1);
    }

    *sharedMemoryPtr = semIDs; // This requires two pointers, e.g., *(*sharedMemoryPtr) + 1

    printf("Enter a number of bakers: ");
    fgets(userInput, sizeof(char) * 10, stdin);

    bakersWorking = atoi(userInput);
    printf("You have entered %d bakers.\n", bakersWorking);

    // Cooks do work
    dispatchStatus = pthread_create(&dispatch, NULL, dispatchBakers, sharedMemoryPtr);
    if (dispatchStatus != 0)
    {
        fprintf(stderr, "Thread create error %d: %s\n", dispatchStatus, strerror(dispatchStatus));
        exit(1);
    }

    dispatchJoinResult = pthread_join(dispatch, &dispatchResult);
    if (dispatchJoinResult != 0)
    {
        fprintf(stderr, "Join error %d: %s\n", dispatchJoinResult, strerror(dispatchJoinResult));
        exit(1);
    }

    // Detachment and Deallocation
    if (shmdt(sharedMemoryPtr) < 0)
    {
        perror("shmdt: Unable to detach\n");
        exit(1);
    }

    if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0)
    {
        perror("shmctl: Unable to deallocate\n");
        exit(1);
    }

    free(userInput);
    return 0;
}

int createSemaphore(int semID) {
    if ((semID = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
        perror("semget: semget failed");
        exit(1);
    }

    return semID;
}

void detachSemaphore(int semID) {
    if (semctl(semID, 0, IPC_RMID) == -1) {
        perror("semctl: semctl failed");
        exit(1);
    }
}

void *dispatchBakers(void *arg){
    for(int bakersDispatched = 0; bakersDispatched < bakersWorking; bakersDispatched++){
        pthread_t baker;
        void *bakerResult;

        int bakerStatus;
        int bakerJoinResult;

        bakerStatus = pthread_create(&baker, NULL, bakerWorks, arg);
        if (bakerStatus != 0)
        {
            fprintf(stderr, "Thread create error %d: %s\n", bakerStatus, strerror(bakerStatus));
            exit(1);
        }
    }   
}

void *bakerWorks(void *arg)
{
    // Baker Test Code
    int32_t* mock = (int*)malloc(sizeof(int) * 10);
    Recipe mockRecipe = {
        .name = "mock",
        .ingredients = {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    Baker myBaker = BakerFactory(mock, 0, 0, mockRecipe);
}