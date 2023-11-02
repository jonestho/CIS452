#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int globalData = 0;

int growingMemorySize = SIZE;
char stack[SIZE];

char *heap;


int main(int argc, char **argv){
    heap = malloc(SIZE);

    printf("Address of Global Data: %p\n", &globalData);
    printf("Address of Stack: %p\n", stack);
    printf("Address of Heap: %p\n", heap);

    return 0;
}