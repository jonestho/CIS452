#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char **argv){
    int numOfBakers;
    char* userInput = (char*) malloc(sizeof(char) * 10); 

    printf("Enter a number of bakers: ");
    fgets(userInput, sizeof(char) * 10, stdin);

    numOfBakers = atoi(userInput);

    printf("You have entered %d bakers.\n", numOfBakers);
    return 0;
}