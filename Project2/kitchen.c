#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct sembuf p = {0, -1, SEM_UNDO}; // wait
struct sembuf v = {0, +1, SEM_UNDO}; // signal

int main(int argc, char **argv)
{
    int sharedMemoryID;
    int utensilSemID, pantrySemID, fridgeSemID, ovenSemID; // utensilSemID is for mixers, bowls, and spoons.

    key_t keySHM = ftok("kitchen.c", 0);
    key_t keySEM = ftok("baker.c", 1);

    int numOfBakers;
    char* userInput = (char*) malloc(sizeof(char) * 10); 

    printf("Enter a number of bakers: ");
    fgets(userInput, sizeof(char) * 10, stdin);

    numOfBakers = atoi(userInput);
    printf("You have entered %d bakers.\n", numOfBakers);



    free(userInput);
    return 0;
}