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

pid_t parentPID;

int main(int argc, char** argv) {
    signal(SIGQUIT, SIG_IGN); // Signal for ending children
    signal(SIGINT, exitProgram); // Signal for ^C ending program
    parentPID = getpid();
    int childNum, pipeResult, badApple, destination;
    char* myMessage = (char*)malloc(sizeof(char) * 500);


    while (1) {
        getUserInput(&childNum, myMessage, &destination, &badApple);
        sendApples(childNum, myMessage, destination, badApple);
    }

    return 0;
}

void getUserInput(int* numChildren, char* userInput, int* destination, int* badApple) {
    char* temp = (char*)malloc(sizeof(char) * 500);
    fflush(stdin);

    printf("Enter a number of nodes: ");
    fgets(temp, 500, stdin);
    *numChildren = atoi(temp);

    printf("Enter a message to send: ");
    fgets(userInput, 500, stdin);
    userInput[strcspn(userInput, "\n")] = 0;


    printf("Which node would you like to send this to? (0 to %d): ", *numChildren - 1);
    fgets(temp, 500, stdin);
    *destination = atoi(temp);

    printf("Which node would you like to be the bad apple? -1 for none or (0 to %d): ", *numChildren - 1);
    fgets(temp, 500, stdin);
    *badApple = atoi(temp);

    free(temp);
}


void sendApples(int childNum, char* myMessage, int destination, int badApple) {
    pid_t pid = 1;
    // Apple theApple = appleFactory(0, destination, 0, 1, myMessage, badApple);
    int readPipe = 0;
    int writePipe = 1;
    char* apple = (char*)malloc(sizeof(char) * 500);
    pid_t* childPIDs = (pid_t*)malloc(sizeof(pid_t) * childNum - 1);
    int hasRead = 0;

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
            signal(SIGQUIT, endChildren);
            readPipe = i;
            writePipe = i + 1;
            strcpy(apple, "/"); // will be the empty string

            if (i == childNum - 1) {
                readPipe = i;
                writePipe = 0;
            }
            printf("PID: [%d] | Node #%d, read(%d) write(%d)\n", getpid(), i, readPipe, writePipe);
            break;
        }
        i++;
    }

    // Creating parent apple
    if (getpid() == parentPID) {
        strcpy(apple, myMessage);
        printf("PID: [%d] | Node #0, read(%d) write(%d)\n", getpid(), 0, 1);
    }

    // Loop for sending messages
    while (1) {
        if (!strcmp(apple, "/")) { // if empty
            read(myPipes[readPipe][0], apple, 500);

            // If the message is NOT empty, then we can close the pipe and print the message
            if (strcmp(apple, "/")) {
                printf("PID: [%d] | Node: [%d] received \"%s\"\n", getpid(), readPipe, apple);

                // Check for bad apple
                if (readPipe == badApple) {
                    for (int i = 0; i < strlen(apple); i++) {
                        if (rand() % 2 == 0)
                            apple[i] = (int)apple[i] ^ rand() % 256;
                    }
                }

                // Check if message has reached the parent
                if (readPipe == 0) {
                    hasRead = 1;
                }

                // Check if message is intended for this node
                if (destination == readPipe) {
                    printf("\033[32mPID: [%d] | Message has reached intended node: [%d]. Received \"%s\"\033[00m\n", getpid(), readPipe, apple);
                    memset(apple, 0, strlen(apple));
                }
            }
        }
        else if (readPipe == 0 && hasRead) { // if node 0 has read a message in
            kill(0, SIGQUIT); // https://stackoverflow.com/questions/18433585/kill-all-child-processes-of-a-parent-but-leave-the-parent-alive
            free(apple);
            int childStatus = 0;
            while ((pid = wait(&childStatus)) > 0); // https://stackoverflow.com/questions/19461744/how-to-make-parent-wait-for-all-child-processes-to-finish
            return;
        }
        else if (!hasRead) {
            if (write(myPipes[writePipe][1], apple, 500) < 0) {
                perror("FAILED TO SEND");
            }
            else {
                printf("PID: [%d] | Node: [%d] wrote \"%s\" to Node [%d]\n", getpid(), readPipe, apple, writePipe);
                strcpy(apple, "/");
            }
        }
    }
}

void endChildren(int sig) {
    printf("PID: [%d] | Child received signal. Ending process\n", getpid());
    exit(0);
}


void exitProgram(int sig) {
    printf("PID: [%d] | Exiting program\n", getpid());
    exit(0);
}