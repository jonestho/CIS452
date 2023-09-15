#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void sigHandlerUsr1(int);
void sigHandlerUsr2(int);
void sigHandlerInt(int);

int main(int argc, char** argv){
    pid_t pid;

    if((pid = fork()) < 0){
        printf("Fork failed:\n");
        exit(1);
    }else if(pid == 0){
        printf("spawned child PID# %d\n", getpid());

        while(1){
            printf("waiting...\n");
            sleep(5);
            kill(getppid(), SIGUSR1);
        }

    }else{
        while(1){
            signal(SIGUSR1, sigHandlerUsr1); signal(SIGUSR2, sigHandlerUsr2); signal(SIGINT, sigHandlerInt);
            pause();
        }
    }

    return 0;
}

void sigHandlerUsr1(int sigNum){
    printf("received a SIGUSR1 signal\n");
}

void sigHandlerUsr2(int sigNum){
    printf("received a SIGUSR2 signal\n");
}

void sigHandlerInt(int sigNum){
    printf(" received. That's it, I'm shutting you down...\n");
    exit(0);
}