#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void signalHandler(int signal);
void *doDispatch(void *arg);
void *doWork(void *arg);

int filesAccessed = 0;

int main(int argc, int argv[]){
    signal(SIGINT, signalHandler);

    pthread_t dispatch;
    void *dispatchResult;
    
    int dispatchStatus;
    int dispatchJoinResult;
    char fileName[500];

    dispatchStatus = pthread_create(&dispatch, NULL, doDispatch, &fileName);
    if(dispatchStatus != 0){
        fprintf(stderr, "Thread create error %d: %s\n", dispatchStatus, strerror(dispatchStatus));
        exit(1);
    }

    dispatchJoinResult = pthread_join(dispatch, &dispatchResult);
    if(dispatchJoinResult != 0 ){
        fprintf(stderr, "Join error %d: %s\n", dispatchJoinResult, strerror(dispatchJoinResult));
        exit(1);
    }

    return 0;
}

void *doDispatch(void *arg){
    while(1){
        pthread_t worker;
        void *workerResult;

        int workerStatus;
        int workerJoinResult;

        printf("Enter a filename: ");
        scanf("%s", arg);

        workerStatus = pthread_create(&worker, NULL, doWork, arg);
        if(workerStatus != 0){
            fprintf(stderr, "Thread create error %d: %s\n", workerStatus, strerror(workerStatus));
            exit(1);
        }

        workerJoinResult = pthread_join(worker, &workerResult);
        if(workerJoinResult != 0 ){
            fprintf(stderr, "Join error %d: %s\n", workerJoinResult, strerror(workerJoinResult));
            exit(1);
        }
    }

    return (void *) arg;
}

void *doWork(void *arg){
    printf("Searching...\n");
    if ((double) rand() / (double)RAND_MAX >= 0.2)
        sleep(1);
    else
        sleep(rand() % 3 + 7);

    printf("File \"%s\" found. Terminating thread\n", arg);
    filesAccessed++;
}

void signalHandler(int signal){
    printf("\nFiles Accessed: (%d) Exiting program.\n", filesAccessed);
    exit(0);
}