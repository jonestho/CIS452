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
    size_t memSize = 18582913020;

    while(1){
        if ((shmID = shmget(IPC_PRIVATE, memSize, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
            perror("Unable to get shared memory\n");
            printf("Max Memory Size: %ld\n", (memSize - 1));
            exit(1);
        }

        if (shmctl(shmID, IPC_RMID, 0) < 0) {
            perror("Unable to deallocate\n");
            exit(1);
        }

        printf("Mem Size: %ld\n", memSize);
        memSize += 1;
    }
    
    return 0;
}