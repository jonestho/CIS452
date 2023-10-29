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

Recipe cookiesRecipe = {
    .name = "Cookies",
    .ingredients = {1, 1, 0, 0, 0, 0, 0, 1, 1}
};

Recipe pancakesRecipe = {
    .name = "Cookies",
    .ingredients = {1, 1, 0, 1, 1, 0, 1, 1, 1}
};

Recipe pizzaDoughRecipe = {
    .name = "Pizza Dough",
    .ingredients = {0, 1, 1, 0, 1, 0, 0, 0, 0}
};

Recipe softPretzelRecipe = {
    .name = "Soft Pretzel",
    .ingredients = {1, 1, 1, 1, 1, 0, 1, 0, 0}
};

Recipe cinnamonRollRecipe = {
    .name = "Cinnamon Roll",
    .ingredients = {1, 1, 0, 0, 1, 1, 1, 0, 1}
};

int createSemaphore(int semID);
void detachSemaphore(int semID);
void *dispatchBakers(void *arg);
void *bakerWorks(void *arg);