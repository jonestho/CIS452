#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{
    char *data1;
    data1 = malloc(SIZE);

    char *data1Pointer = &data1;

    printf("Please input username: ");
    scanf("%s", data1);

    printf("You entered: [%s]\n", data1);
    free(data1);
    return 0;
}