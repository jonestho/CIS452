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

int main(int argc, char** argv) {
    pid_t pid;
    int status;
    command_t commands;
    struct rusage CPUStat;

    const char* quitString = "quit";
    char* userInput = (char*)calloc(100, sizeof(char));
    buildCommands(&commands);

    printf("Welcome to the simple shell. Type 'quit' to exit.\n");
    while (0 != strcmp(userInput, quitString)) {
        printf("\n> ");

        // Reading stdin and tokenizing it
        fgets(userInput, 100, stdin);
        userInput[strcspn(userInput, "\n")] = 0;
        tokenizeUserInput(&commands, userInput);

        // Using the tokenized stsring to perform commands
        if (0 != strcmp(userInput, quitString)) {
            if ((pid = fork()) < 0) {
                fprintf(stderr, "Fork failure: %s", strerror(errno));
                exit(1);
            }
            else if (pid == 0) {
                if (execvp(commands.commands[0][0], commands.commands[0]) < 0) {
                    perror("\e[0;31mexec failed\e[0m");
                    exit(1);
                }
                // printf("\n");
            }
            else {
                wait(&status);
                getrusage(RUSAGE_CHILDREN, &CPUStat);
                printf("User CPU Time Used: %ld (μs)\n", CPUStat.ru_stime.tv_usec);
                printf("# of involuntary context switches: %ld\n", CPUStat.ru_nivcsw);
            }
        }
    }


    free(userInput);
    freeCommands(&commands);
    return 0;
}


/**
 * @brief used to dynmaically tokenize the input of the user. Input should be seperated by spaces.
 *
 *
 * @param args where tokenized outputs are stored. ARGS WILL BE DESTROYED
 */


void tokenizeUserInput(command_t* commands, char* line) {
    // Reset commands
    freeCommands(commands);
    buildCommands(commands);

    char* token = strtok_r(line, ";", &line);
    int counter = 0;

    while (token != NULL) {
        // Tokenize each command into individual arguments
        char* tokenCopy = strdup(token);
        char* argToken = strtok_r(tokenCopy, " ", &tokenCopy);
        int argsCounter = 0;

        // if there are arguments -> the -a in 'ls -a'
        if (argToken != NULL) {
            while (argToken != NULL) {
                commands->commands[counter] = realloc(commands->commands[counter], (argsCounter + 1) * sizeof(char*));
                commands->commands[counter][argsCounter] = strdup(argToken);
                argsCounter++;
                argToken = strtok_r(NULL, " ", &tokenCopy);
            }
        }

        // This allows multiple args to work for some reason?
        commands->commands[counter][argsCounter] = NULL;
        commands->numCommands++;

        token = strtok_r(NULL, ";", &line);
        counter++;
    }

}

void freeCommands(command_t* commands) {
    if (commands != NULL) {
        // free all commands within the struct
        for (int i = 0; i < commands->numCommands; i++) {
            free(commands->commands[i]);
        }

        // free the pointer to the string array
        if (commands->numCommands)
            free(commands->commands);
    }

}

void buildCommands(command_t* commands) {
    commands->commands = (char***)calloc(1, sizeof(char**));
    commands->numCommands = 0;
}