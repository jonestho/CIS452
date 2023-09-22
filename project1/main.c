#include "main.h"


int main(int argc, char** argv) {
    signal(SIGUSR1, endProgram); // Signal for ending program
    signal(SIGINT, endProgram); // Signal for ^C ending program

    pid_t pid;
    int childNum, pipeResult, destination, badApple;
    char* myMessage = (char*)malloc(sizeof(char) * 500);
    Apple theApple;
    getUserInput(&childNum, myMessage, &destination, &badApple);

    // Creating and instantiating pipes
    int myPipes[childNum][2];
    for (int i = 0; i < childNum; i++) {
        if (pipe(myPipes[i]) < 0) {
            perror("Failed to make pipes");
            exit(1);
        }
    }


    // Loop for creating children and apples
    int i = 1;
    while (i < childNum && pid > 0) {
        if ((pid = fork()) < 0) {
            printf("Failed creating child #%d\n", childNum + 1);
            exit(1);
        }
        else if (pid == 0) {
            signal(SIGUSR1, endChild);
            theApple = appleFactory(i, destination, i, i + 1, "", badApple);

            if (i == childNum - 1) {
                theApple.writePipe = 0;
            }
            printf("PID: [%d] | Child #%d, read(%d) write(%d)\n", getpid(), theApple.readPipe, theApple.readPipe, theApple.writePipe);
            break;
        }
        i++;
    }

    // Creating parent apple
    if (pid > 0) {
        theApple = appleFactory(0, destination, 0, 1, myMessage, badApple);
    }


    struct stat pipeStat;

    // Loop for sending messages
    while (1) {
        if (!strcmp(theApple.message, "")) { // if empty
            read(myPipes[theApple.readPipe][0], theApple.message, 500);

            // If the message is NOT empty, then we can close the pipe and print the message
            if (strcmp(theApple.message, "")) {
                printf("PID: [%d] | Node: [%d] received \"%s\"\n", getpid(), theApple.ID, theApple.message);
                close(myPipes[theApple.readPipe][0]);
            }

            // Check for bad apple
            if (theApple.readPipe == theApple.badApple) {
                for (int i = 0; i < strlen(theApple.message); i++) {
                    if (rand() % 2 == 0)
                        theApple.message[i] = (int)theApple.message[i] ^ rand() % 256;
                }
            }

            // Check if message is intended for this node
            if (theApple.receiver == theApple.readPipe) {
                printf("\033[32mPID: [%d] Message has reached intended node: [%d]. Received \"%s\"\033[00m\n", getpid(), theApple.ID, theApple.message);
                raise(SIGUSR1);
            }
        }
        else {
            if (write(myPipes[theApple.writePipe][1], theApple.message, 500) < 0) {
                perror("FAILED TO SEND");
            }
            close(myPipes[theApple.writePipe][1]);
            strcpy(theApple.message, "\0");

            printf("PID: [%d] | Apple [%d] wrote to Apple: %d\n", getpid(), theApple.ID, theApple.writePipe);
        }
    }


    free(myMessage);
    return 0;
}

void getUserInput(int* numChildren, char* userInput, int* dest, int* badApple) {
    char* temp = (char*)malloc(sizeof(char) * 500);

    printf("Enter a number of children: ");
    fgets(temp, 500, stdin);
    *numChildren = atoi(temp);

    printf("Enter a message to send: ");
    fgets(userInput, 500, stdin);
    userInput[strcspn(userInput, "\n")] = 0;


    printf("Which child would you like to send this to? (0 to %d): ", *numChildren - 1);
    fgets(temp, 500, stdin);
    *dest = atoi(temp);

    printf("Which child would you like to be the bad apple? -1 for none or (0 to %d): ", *numChildren - 1);
    fgets(temp, 500, stdin);
    *badApple = atoi(temp);

    free(temp);
}

Apple appleFactory(int ID, int receiver, int readPipe, int writePipe, char* message, int badApple) {

    Apple myApple;
    myApple.ID = ID;
    myApple.receiver = receiver;
    myApple.readPipe = readPipe;
    myApple.writePipe = writePipe;
    strcpy(myApple.message, message);
    myApple.badApple = badApple;

    return myApple;
}


void endProgram(int sig) {
    printf("PID: [%d] | Parent received signal. Ending processes\n", getpid());
    int process_group_id = getpgrp();
    kill(-process_group_id, SIGUSR1);
    signal(SIGUSR1, exitProgram);
}

void endChild(int sig) {
    printf("PID: [%d] | Child received signal. Ending process\n", getpid());
    kill(getppid(), SIGUSR1);
    exit(0);
}

void exitProgram(int sig) {
    printf("PID: [%d] | Parent received final signal. Exiting program\n", getpid());
    sleep(1);
    exit(0);
}