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
    int numOfBakers;
    char* userInput = (char*) malloc(sizeof(char) * 10); 

    printf("Enter a number of bakers: ");
    fgets(userInput, sizeof(char) * 10, stdin);

    numOfBakers = atoi(userInput);
    printf("You have entered %d bakers.\n", numOfBakers);

    int utensilSemID, fridgeSemID, pantrySemID, ovenSemID;

    utensilSemID = createSemaphore(utensilSemID);
    fridgeSemID = createSemaphore(fridgeSemID);
    pantrySemID = createSemaphore(pantrySemID);
    ovenSemID = createSemaphore(ovenSemID);

    free(userInput);
    return 0;
}

int createSemaphore(int semID){
    if ((semID = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    return semID;
}