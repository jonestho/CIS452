#include "main.h"


typedef struct {
    char* message;
    int receiver;
} apple;


int main(int argc, char** argv) {

    int fd[3], pid, outputLength = 50;
    int pipeCreationsResult = pipe(fd);

    char output[50] = "Schmeeble";
    char input[50];

    if (pipeCreationsResult < 0) {
        perror("Failed pipe creation\n");
        exit(1);
    }

    pid = fork();

    // spawning the circle
    int processCounter = 2; // set to 2 because the child needs to spawn the fork, so 2 children already exist
    while (processCounter < 3 && pid == 0) {
        if ((pid = fork()) < 0) {
            printf("Failed Child creation on child: %d\n", processCounter);
            exit(1);
        }

        processCounter++;
    }

    // if child
    if (pid == 0) {
        close(fd[1]);

        read(fd[0], input, outputLength);
        printf("Child received: [%s] from parent\n", input);

        //free(output);
    }
    else { // parent
        close(fd[0]);

        write(fd[1], output, outputLength);
        printf("Parent sent: [%s] \n", output);
        close(fd[1]);

        //free(input);
    }



    return 0;

}

