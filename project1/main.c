#include "main.h"
#include <sys/types.h>


typedef struct {
    char message[500];
    int receiver;
    
    int readPipe;
    int writePipe;
} apple;

int main(int argc, char** argv) {
    pid_t pid;
    apple theApple;
   
    int childNum;
    
    printf("Enter a number of children: ");
    scanf("%d", &childNum);

    int children[childNum];
    int fileDirectors[childNum + 1][3];

    theApple.readPipe = childNum;
    theApple.writePipe = 0;


    int pipeResult;

    for(int i=0; i <= childNum; i++){
        pipeResult = pipe(fileDirectors[i]);

        if(pipeResult < 0){
            printf("Failed to create pipe\n");
            exit(1);
        }
    }

    int fdArray[3];
    pipeResult = pipe(fdArray);

    if(pipeResult < 0){
        printf("Failed to create pipe\n");
        exit(1);
    }

    int i = 0;
    int childPID;

    // REMOVE WHEN READY
    printf("Parent PID: %d, read(%d), write(%d)\n", getpid(), theApple.readPipe, theApple.writePipe);

    pid = getpid();

    while (i < childNum && pid > 0) {
        if ((pid = fork()) < 0) {
            printf("Failed creating child #%d\n", childNum + 1);
            exit(1);
        }else if(pid == 0){
            int toSend = getpid();
            theApple.readPipe = i;
            theApple.writePipe = i + 1;

            // REMOVE WHEN READY
            printf("Child #%d: %d, read(%d) write(%d)\n", i + 1, getpid(), theApple.readPipe, theApple.writePipe);
            write(fdArray[1], &toSend, sizeof(toSend));
        }else{
            read(fdArray[0], &childPID, sizeof(int));
            children[i] = childPID;

            i++;
        }
    }

    if(pid > 0){
        printf("Children spawned successfully.\n\n");
        close(fdArray[0]); close(fdArray[1]);

        printf("Enter a message to send: ");
        scanf("%s", theApple.message);

        printf("Which child would you like to send this to? (0 to %d): ", childNum - 1);
        scanf("%d", &theApple.receiver);
    }

    return 0;
}
