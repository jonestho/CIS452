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

#define LOCAL_HOST "127.0.0.1"   
#define MAX_CLIENTS 5
int listenForClient();
int* getPrimeFactors(int ID, int number);