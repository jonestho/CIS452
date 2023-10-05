#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main(int argc, char *argv[])
{
    struct sembuf p = {0, -1, SEM_UNDO}; // wait
    struct sembuf v = {0, +1, SEM_UNDO}; // signal

    int status;
    long int i, loop, temp, *sharedMemoryPointer;
    int sharedMemoryID, semID;

    pid_t pid;

    loop = atoi(argv[1]);
    sharedMemoryID = shmget(IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR);

    if (sharedMemoryID < 0)
    {
        perror("Unable to obtain shared memory\n");
        exit(1);
    }

    sharedMemoryPointer = shmat(sharedMemoryID, 0, 0);

    if (sharedMemoryPointer == (void *)-1)
    {
        perror("Unable to attach\n");
        exit(1);
    }

    sharedMemoryPointer[0] = 0;
    sharedMemoryPointer[1] = 1;

    temp = sharedMemoryPointer[1];

    if ((semID = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR | IPC_CREAT)) == -1)
    {
        perror("semget: semget failed");
        exit(1);
    }

    if (semctl(semID, 0, SETVAL, 1) < 0)
    {
        perror("semctl: semctl failed");
        exit(1);
    }

    pid = fork();
    if (pid < 0)
    {
        printf("Fork failed\n");
    }

    if (pid == 0)
    {
        // Child

        if (semop(semID, &p, 1) < 0)
        { // wait op
            perror("semop: wait op failed");
            exit(1);
        }

        for (i = 0; i < loop; i++)
        {
            temp = sharedMemoryPointer[1];
            
            sharedMemoryPointer[1] = sharedMemoryPointer[0];
            sharedMemoryPointer[0] = temp;
        }

        if (semop(semID, &v, 1) < 0)
        { // signal op
            perror("semop: wait op failed");
            exit(1);
        }

        if (shmdt(sharedMemoryPointer) < 0)
        {
            perror("Unable to detach\n");
            exit(1);
        }
        exit(0);
    }
    else
        if (semop(semID, &p, 1) < 0)
        { // wait op
            perror("semop: wait op failed");
            exit(1);
        }

        for (i = 0; i < loop; i++)
        {
            temp = sharedMemoryPointer[0];
            
            sharedMemoryPointer[0] = sharedMemoryPointer[1];
            sharedMemoryPointer[1] = temp;
        }

        if (semop(semID, &v, 1) < 0)
        { // signal op
            perror("semop: wait op failed");
            exit(1);
        }

        wait(&status);

        printf("Values: %li\t%li\n", sharedMemoryPointer[0], sharedMemoryPointer[1]);

        if (shmdt(sharedMemoryPointer) < 0)
        {
            perror("Unable to detach\n");
            exit(1);
        }

        if (shmctl(sharedMemoryID, IPC_RMID, 0) < 0)
        {
            perror("Unable to deallocate\n");
            exit(1);
        }

        if((semctl(semID, 0, IPC_RMID)) < 0){
            perror("semctl: unable to remove");
            exit(1);
        }

        return 0;
}