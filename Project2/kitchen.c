// #pragma once
#include "kitchen.h"

int main(int argc, char** argv) {
    // int numOfBakers;
    // char* userInput = (char*) malloc(sizeof(char) * 10); 
    int32_t* mock = (int*)malloc(sizeof(int) * 10);
    Recipe mockRecipe = {
        .name = "mock",
        .ingredients = {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    // printf("Enter a number of bakers: ");
    // fgets(userInput, sizeof(char) * 10, stdin);

    // numOfBakers = atoi(userInput);
    // printf("You have entered %d bakers.\n", numOfBakers);

    Baker myBaker = BakerFactory(mock, 0, 0, mockRecipe);

    int semIDs[4]; // pantrySemID, fridgeSemID, utensilSemID, ovenSemID, 
    semIDs[0] = createSemaphore(semIDs[0]); // pantrySemID
    semIDs[1] = createSemaphore(semIDs[1]); // fridgeSemID
    semIDs[2] = createSemaphore(semIDs[2]); // utensilSemID
    semIDs[3] = createSemaphore(semIDs[3]); // ovenSemID

    // free(userInput);
    free(mock);
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