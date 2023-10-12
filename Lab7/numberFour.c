#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

int main(int argc, char **argv){
    int shmID;
    int memSize = 2147483000;

    while(1){
        if ((shmID = shmget(IPC_PRIVATE, memSize, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
            perror("Unable to get shared memory\n");
            printf("Max Memory Size: %d\n", memSize);
            exit(1);
        }

        if (shmctl(shmID, IPC_RMID, 0) < 0) {
            perror("Unable to deallocate\n");
            exit(1);
        }

        printf("Mem Size: %d\n", memSize);
        memSize += 500;
    }
    
    return 0;
}