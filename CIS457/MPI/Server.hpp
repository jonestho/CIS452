#include <mpi.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 5000
#define LOCALHOST "127.0.0.1"   
#define MAX_CLIENTS 5

int listenForClient();
int* getPrimeFactors(int ID, int number);