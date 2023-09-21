#include "main.h"


int main(int argc, char** argv) {
    signal(SIGUSR1, endProgram);
    pid_t pid;
    int childNum, pipeResult, destination;
    char* myMessage = (char*)malloc(sizeof(char) * 500);
    Apple theApple;
    getUserInput(&childNum, myMessage, &destination);

    int myPipes[childNum][2];

    for (int i = 0; i < childNum; i++) {
        if (pipe(myPipes[i]) < 0) {
            perror("Failed to make pipes");
            exit(1);
        }
    }

    int i = 0;
    int childPID;

    // REMOVE WHEN READY
    // printf("Parent PID: %d, read(%d), write(%d)\n", getpid(), theApple.readPipe, theApple.writePipe);

    pid = getpid();
    i = 1;
    // Loop for creating children and apples
    while (i < childNum && pid > 0) {
        if ((pid = fork()) < 0) {
            printf("Failed creating child #%d\n", childNum + 1);
            exit(1);
        }
        else if (pid == 0) {
            signal(SIGUSR1, endChild);
            theApple = appleFactory(i, destination, i, i + 1, "");

            if (i == childNum - 1) {
                theApple.writePipe = 0;
            }

            // REMOVE WHEN READY
            // printf("Child #%d: %d, read(%d) write(%d)\n", theApple.readPipe, getpid(), theApple.readPipe, theApple.writePipe);
            break;
        }
        i++;
    }

    if (pid > 0) {
        theApple = appleFactory(0, destination, 0, 1, myMessage);
        // printf("Parent #%d: %d, read(%d) write(%d)\n", theApple.ID, getpid(), theApple.readPipe, theApple.writePipe);
    }

    // printf("child: %d, reads from %d writes to %d\n", theApple.ID, theApple.readPipe, theApple.writePipe);

    struct stat pipeStat;

    // Loop for sending messages
    while (1) {
        if (!strcmp(theApple.message, "") || theApple.sent == 1) {
            read(myPipes[theApple.readPipe][0], theApple.message, 500);

            if (strcmp(theApple.message, "")) {
                close(myPipes[theApple.readPipe][0]);

                if (theApple.receiver == theApple.readPipe) {
                    printf("PID: [%d] Child: [%d] received \"%s\"\n", getpid(), theApple.ID, theApple.message);
                    raise(SIGUSR1);
                }
            }
        }
        else {
            if (write(myPipes[theApple.writePipe][1], theApple.message, 500) < 0) {
                perror("FAILED TO SEND");
            }
            close(myPipes[theApple.writePipe][1]);

            printf("PID: [%d] | Apple [%d] wrote to Apple: %d\n", getpid(), theApple.ID, theApple.writePipe);
            theApple.sent = 1;
        }
    }

    free(myMessage);
    return 0;
}

void getUserInput(int* numChildren, char* message, int* dest) {
    printf("Enter a number of children: \n");
    scanf("%d", numChildren);

    printf("Enter a message to send: \n");
    scanf("%s", message);

    printf("Which child would you like to send this to? (0 to %d): \n", *numChildren - 1);
    scanf("%d", dest);
}

Apple appleFactory(int ID, int receiver, int readPipe, int writePipe, char* message) {

    Apple myApple;
    myApple.ID = ID;
    myApple.receiver = receiver;
    myApple.readPipe = readPipe;
    myApple.writePipe = writePipe;
    strcpy(myApple.message, message);
    myApple.sent = 0;

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
    printf("PID: [%d] | Parent received final signal. Ending processes\n", getpid());
    sleep(1);
    exit(0);
}