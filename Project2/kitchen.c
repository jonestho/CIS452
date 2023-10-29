// #pragma once
#include "kitchen.h"

int bakersWorking;
int **sharedMemoryPtr;

int main(int argc, char** argv) {
    pthread_t dispatch;
    void *dispatchResult;

    int dispatchStatus;
    int dispatchJoinResult;

    int sharedMemoryID;
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
    int *numOfBakersPtr;
    numOfBakersPtr = (int*)malloc(sizeof(int));
    memcpy(numOfBakersPtr, &bakersWorking, sizeof(int));

    dispatchStatus = pthread_create(&dispatch, NULL, dispatchBakers, numOfBakersPtr);
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
    int* numOfBakers = (int*) arg;

    for(int i = 0; i < *numOfBakers; i++){
        pthread_t baker;
        void *bakerResult;
        int bakerStatus;

        // TODO: Make args and array that includes bakerID and ramsay. It will be handled in the baker thread.
        // - Have the Ramsay value be random or from the user.
        // - Ramsay code should probably be handled near the end of the project.

        int *bakerID;
        bakerID = (int*)malloc(sizeof(int));
        memcpy(bakerID, &i, sizeof(int));

        bakerStatus = pthread_create(&baker, NULL, bakerWorks, bakerID);
        if (bakerStatus != 0)
        {
            fprintf(stderr, "Thread create error %d: %s\n", bakerStatus, strerror(bakerStatus));
            exit(1);
        }
    }

    while(bakersWorking > 0);

    printf("Bakers are finished.\n");
}

void *bakerWorks(void *arg)
{
    int *bakerID = (int*) arg;
    printf("Baker %d is cooking \n", *bakerID);

    /*
    // Baker Mockup Code
    int32_t* mock = (int*)malloc(sizeof(int) * 10);
    Recipe mockRecipe = {
        .name = "mock",
        .ingredients = {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    
    Baker myBaker = BakerFactory(mock, 0, 0, mockRecipe);

    free(mock);
    
    // Baker cooks all the recipes (Refer to kitchen.h for recipes)
    // arg = ID of Baker
    myBaker = BakerFactory(sharedMemoryPtr, bakerID, 0, cookiesRecipe);
    myBaker = BakerFactory(sharedMemoryPtr, bakerID, 0, pancakesRecipe);
    myBaker = BakerFactory(sharedMemoryPtr, bakerID, 0, pizzaDoughRecipe);
    myBaker = BakerFactory(sharedMemoryPtr, bakerID, 0, softPretzelRecipe);
    myBaker = BakerFactory(sharedMemoryPtr, bakerID, 0, softPretzelRecipe);
    */

    bakersWorking--;
}