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

#define LOCAL_HOST "127.0.0.1"   
#define MAX_CLIENTS 5
void listenForClient(int ranks);
int* getPrimeFactors(int ID, int number);
std::vector<int> parseMessage(char* buffer);