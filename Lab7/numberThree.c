
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>


int main() {
    int semaphore;
    struct sembuf v = { 0, +1, SEM_UNDO };

    int counter = 0;
    if ((semaphore = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1) {
        perror("semget failed");
        for (int i = 0; i < counter; i++)
            exit(1);
    }

    while (1) {
        if (semop(semaphore, &v, 1) == -1) {
            perror("semop failed\n");
            exit(1);
        }
        counter++;
        printf("semaphore: %d\n", counter);
    }

    semctl(semaphore, 0, IPC_RMID, 0);



    return 0;
}