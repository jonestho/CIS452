#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

void getUserInput(int* numChildren, char* message, int* destination, int* badApple);
void sendApples(int childNum, char* myMessage, int destination, int badApple);
void endChildren(int sig);
void exitProgram(int sig);
