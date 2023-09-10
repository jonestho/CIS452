#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>

typedef struct {
    char*** commands;
    int numCommands;
} command_t;

void tokenizeUserInput(command_t* commands, char* line);
void freeCommands(command_t* commands);
void buildCommands(command_t* commands);
int findNumCommands(char* line);