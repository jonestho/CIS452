#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char* data1;
    size_t size = 16;
    data1 = (char*)malloc(sizeof(char) * 16);

    printf("Please input username: ");

    // gets the line from user and automatically allocates memory
    int lineSize = getline(&data1, &size, stdin); 

    // remove the \n from when you press 'enter'
    memset(&data1[lineSize-1], '\0', 1); 

    printf("You entered: [%s]\n", data1);
    free(data1);

    return 0;
}