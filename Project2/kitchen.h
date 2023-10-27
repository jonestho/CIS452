#pragma once

#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdint.h>

struct sembuf p = {0, -1, SEM_UNDO}; // wait
struct sembuf v = {0, +1, SEM_UNDO}; // signal

#include "baker.h"
#include "baker.c"


int createSemaphore(int semID);
void detachSemaphore(int semID);