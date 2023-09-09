#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tokenizeUserInput(char** args, char* line);
void freeArgs(char** args);

int main(int argc, char** argv) {
    char* userInput = (char*)malloc(100 * sizeof(char));
    char** commands;


    printf(" __   __  _     _  __   __    _______  __   __  _______  ___      ___        __   __  _______        ____ \n");
    printf("|  | |  || | _ | ||  | |  |  |       ||  | |  ||       ||   |    |   |      |  | |  ||  _    |      |    |\n");
    printf("|  | |  || || || ||  | |  |  |  _____||  |_|  ||    ___||   |    |   |      |  |_|  || | |   |       |   |\n");
    printf("|  |_|  ||       ||  |_|  |  | |_____ |       ||   |___ |   |    |   |      |       || | |   |       |   |\n");
    printf("|       ||       ||       |  |_____  ||       ||    ___||   |___ |   |___   |       || |_|   | ___   |   |\n");
    printf("|       ||   _   ||       |   _____| ||   _   ||   |___ |       ||       |   |     | |       ||   |  |   |\n");
    printf("|_______||__| |__||_______|  |_______||__| |__||_______||_______||_______|    |___|  |_______||___|  |___|\n");
    printf("\n\n\n\n\n\n\n\n\n");


    while (0 != strcmp(userInput, "quit")) {
        printf("\n> ");

        fgets(userInput, 100, stdin);
        tokenizeUserInput(commands, userInput);
    }


    free(userInput);
    return 0;
}


/**
 * @brief used to dynmaically tokenize the input of the user. Input should be seperated by spaces.
 *
 *
 * @param args where tokenized outputs are stored. ARGS WILL BE DESTROYED
 */

void tokenizeUserInput(char** args, char* line) {
    char* token = (char*)malloc(100 * sizeof(char));
    args = (char**)malloc(10 * sizeof(char*));
    token = strtok(line, " ");

    int counter = 0;
    while (token != NULL) {

        // reallocates memory if necessary
        if (counter > 10) {
            args = realloc(args, counter * sizeof(char*));
        }
        // allocates memory for new command and copies to array
        args[counter] = (char*)malloc(10 * sizeof(char));
        memcpy(args[counter], token, strlen(token));

        // Grabs next token
        token = strtok(NULL, " ");
    }


}

// WIP
void freeArgs(char** args) {
    int argsLength = sizeof(args) / 8;
    printf("Length %d\n", argsLength);

    for (int i = 0; i < argsLength; i++) {
        free(args[i]);
    }
    free(args);

}