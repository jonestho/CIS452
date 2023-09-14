

void signalHandlerSIGUSR1();
void signalHandlerSIGUSR2();

int main(int argc, char** argv){
    signal(SIGUSR1, signalHandlerSIGUSR1);
    signal(SIGUSR2, signalHandlerSIGUSR2);

    int pid;

    pid = fork();


    if(pid < 0){
        perror("Failed to fork");
        exit(1);
    }
    if(pid == 0){
        exit(0)
    }

    return 0;
}


void signalHandlerSIGUSR1(){


}


void signalHandlerSIGUSR2(){


}
