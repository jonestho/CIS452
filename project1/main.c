#include "main.h"
#include <sys/types.h>


typedef struct {
    char message[500];
    int receiver;
} apple;


int main(int argc, char** argv) {
    pid_t pid;
    apple theApple;
   
    int childNum;
    
    printf("Enter a number of children: ");
    scanf("%d", &childNum);

    int children[childNum];
    int fdArray[3];

    int pipeResult = pipe(fdArray);
    if(pipeResult < 0){
        printf("Failed to create pipe\n");
        exit(1);
    }

    int i = 0;
    int childPID;

    pid = getpid();

    while (i < childNum && pid > 0) {
        if ((pid = fork()) < 0) {
            printf("Failed creating child #%d\n", childNum + 1);
            exit(1);
        }else if(pid == 0){
            int toSend = getpid();
            write(fdArray[1], &toSend, sizeof(toSend));
        }else{
            read(fdArray[0], &childPID, sizeof(int));
            children[i] = childPID;

            i++;
        }
    }

    // This code runs from the parent process
    if(pid > 0){
        printf("Children spawned successfully.\n\n");

        printf("Enter a message to send: ");
        scanf("%s", theApple.message);

        printf("Which child would you like to send this to? (0 to %d): ", childNum - 1);
        scanf("%d", &theApple.receiver);
    }

    return 0;
}
