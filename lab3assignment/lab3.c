#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

void sigHandlerSIGUSR1(int);
void sigHandlerSIGUSR2(int);
void sigHandlerINT(int);

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
            signal(SIGUSR1, sigHandlerSIGUSR1); signal(SIGUSR2, sigHandlerSIGUSR2); signal(SIGINT, sigHandlerINT);
            pause();
        }
    }

    return 0;
}

void sigHandlerSIGUSR1(int sigNum){
    printf("received a SIGUSR1 signal\n");
}

void sigHandlerSIGUSR2(int sigNum){
    printf("received a SIGUSR2 signal\n");
}

void sigHandlerINT(int sigNum){
    printf(" received. That's it, I'm shutting you down...\n");
    exit(0);
}