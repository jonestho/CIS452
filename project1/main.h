#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>


typedef struct {
    int ID;
    int receiver;
    int readPipe;
    int writePipe;
    char message[500];
    int badApple;
} Apple;



void getUserInput(int* numChildren, char* message, int* dest, int* badApple);
Apple appleFactory(int ID, int receiver, int readPipe, int writePipe, char* message, int badApple);
void endProgram(int sig);
void endChild(int sig);
void exitProgram(int sig);