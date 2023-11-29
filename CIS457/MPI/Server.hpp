#include <mpi.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <cstring>
#include <vector>
#include <assert.h>
#include <math.h>
#include <pthread.h>

#define LOCAL_HOST "127.0.0.1"   
#define MAX_CLIENTS 5
int listenForClient();
int* getPrimeFactors(uint64_t ID, uint64_t number);
uint64_t* parseMessage(char* buffer);
void* sendToClient(void* client_sockfd);
void runServer(int client_sockfd, int ranks);