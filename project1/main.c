#include "main.h"


typedef struct {
    char* message;
    int receiver;
} apple;


int main(int argc, char** argv) {

    int fd[3], pid, outputLength;
    int pipeCreationsResult = pipe(fd);

    if (pipeCreationsResult < 0) {
        perror("Failed pipe creation\n");
        exit(1);
    }

    pid = fork();

    // spawning the circle
    int processCounter = 2; // set to 2 because the child needs to spawn the fork, so 2 children already exist
    while (processCounter < 3 && pid == 0) {
        if ((pid = fork()) < 0) {
            perror("Failed Child creation on child: %d\n", processCounter);
            exit(1);
        }
    }

    // if child
    if (pid == 0) {
        write(fd[1], output, outputLength);
        printf("Child wrote: [%s] to fd[]\n", output);
        free(output);
    }
    else { // parent
        read(fd[0], input, outputLength);
        printf("Parent receieved: [%s] from \n", input);
        free(input);
    }



    return 0;

}

