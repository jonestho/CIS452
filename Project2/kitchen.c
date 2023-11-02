// #pragma once
#include "kitchen.h"

int* sharedMemoryPtr;

int main(int argc, char** argv) {
    signal(SIGINT, sigtermHandler);
    pthread_t dispatchThread;
    int sharedMemoryID;
    int semIDs[4]; // pantrySemID, fridgeSemID, utensilSemID, ovenSemID
    int bakersWorking;
    char* userInput = (char*)malloc(sizeof(char) * 10);

    // Creating Semaphores
    semIDs[0] = createSemaphore(); // pantrySemID
    semIDs[1] = createSemaphore(); // fridgeSemID
    semIDs[2] = createSemaphore(); // utensilSemID
    semIDs[3] = createSemaphore(); // ovenSemID

    semctl(semIDs[0], 0, SETVAL, 1);
    semctl(semIDs[1], 0, SETVAL, 2);
    semctl(semIDs[2], 0, SETVAL, 2);
    semctl(semIDs[3], 0, SETVAL, 1);

    // Create Shared Memory for Semaphores
    

    if ((sharedMemoryID = shmget(IPC_PRIVATE, sizeof(semIDs), S_IRUSR | S_IWUSR | IPC_CREAT)) < 0) {
        perror("shmget: Unable to obtain shared memory\n");
        exit(1);
    }
    sharedMemoryPtr = shmat(sharedMemoryID, 0, 0);
    memcpy(sharedMemoryPtr, semIDs, sizeof(semIDs));

    // Get user input
    printf("Enter a number of bakers: ");
    fgets(userInput, sizeof(char) * 10, stdin);
    bakersWorking = atoi(userInput);
    printf("You have entered %d bakers.\n", bakersWorking);

    // Create new thread
    if (pthread_create(&dispatchThread, NULL, dispatchBakers, &bakersWorking) != 0) {
        perror("Failed to create dispatch thread");
        exit(1);
    }

    // Join thread
    if (pthread_join(dispatchThread, NULL) != 0) {
        perror("Failed to join dispatch thread");
        exit(1);
    }

    for (int i = 0; i < 4; i++) {
        if(semctl(semIDs[i], 0, IPC_RMID) == -1)
            perror("semctl: semctl failed");
    }

    // Detachment and Deallocation
    if (shmdt(sharedMemoryPtr) < 0) {
        perror("shmdt: Unable to detach\n");
        exit(1);
    }

    // Delete shared memory
    if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0) {
        perror("shmctl: Unable to deallocate\n");
        exit(1);
    }

    // Detach semaphores
    free(userInput);
    return 0;
}



/**
 * @brief Helper for creating semaphores
 * 
 * @param semID 
 * @return int 
 */

int createSemaphore() {
    int semID;
    if ((semID = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1) {
        perror("semget: semget failed");
        exit(1);
    }

    return semID;
}

/**
 * @brief Helper for detaching semaphores
 * 
 *
 * @param semID 
 */

void detachSemaphore(int semID) {
    if (semctl(semID, 0, IPC_RMID) == -1) {
        perror("semctl: semctl failed");
        exit(1);
    }
}

/**
 * @brief Baker driver function
 * 
 * @param arg 
 * @return void* 
 */

void* dispatchBakers(void* arg) {
    int numOfBakers = *(int*)arg;
    int ramsay = rand() % numOfBakers;

    pthread_t* bakerThreads = (pthread_t*)malloc(sizeof(pthread_t) * numOfBakers);
    Baker* bakers = (Baker*)malloc(sizeof(Baker) * numOfBakers);

    // Create Baker Threads
    for (int i = 0; i < numOfBakers; i++) {
        bakers[i] = BakerFactory(sharedMemoryPtr, i, ramsay);

        if (pthread_create(&bakerThreads[i], NULL, bakerWorks, &bakers[i]) != 0) {
            perror("Failed to create baker thread");
            exit(1);
        }
    }

    // Join Baker Threads
    for (int i = 0; i < numOfBakers; i++) {
        if (pthread_join(bakerThreads[i], NULL) != 0) {
            perror("Failed to join baker thread");
            exit(1);
        }
    }

    free(bakerThreads);
    free(bakers);
    printf("Bakers are finished.\n");
}

/**
 * @brief Baker function for threads
 * 
 *
 * @param arg 
 * @return NULL
 */

void* bakerWorks(void* arg) {
    Baker baker = *(Baker*)arg;

    while (baker.currentRecipe < 5) {
        checkRecipe(&baker);
        if (baker.ifCheckFridge) enterFridge(&baker);
        if (baker.ifCheckPantry) enterPantry(&baker);
        useMixer(&baker);

        if (baker.ramsay == baker.ID) {
            printf("Baker %d was Ramsay'd!\n", baker.ID);
            baker.ramsay = -1;
            continue;
        }
        useOven(&baker);
        baker.currentRecipe++;
    }
    printf("Baker %d is done.\n", baker.ID);
    return NULL;
}


void sigtermHandler(int sig) {
    printf("SIGTERM received.\n");

    for (int i = 0; i < 4; i++)
        detachSemaphore(sharedMemoryPtr[i]);

    exit(0);
}


