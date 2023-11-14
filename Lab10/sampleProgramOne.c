#include <stdio.h>
#include <stdlib.h>

#define COEFFICIENT 64
#define KB 1024
#define LOOP 1

int main(){
    int count, *intPtr;
    long int i, j, dim = COEFFICIENT * KB;

    long int memory = 16435429376;
    long int swap = 2147479552;
    long int cap = memory + swap;

    long int projectedMemory = dim * dim * sizeof(int);
    
    if ((intPtr = malloc ((dim * dim * sizeof(int)))) == 0) 
    {
        perror ("totally out of space");
        exit (1);
    }
    
    for (count=1; count<=LOOP; count++)
        for (i=0; i<dim; i++)
            for (j=0; j<dim; j++)
                intPtr[i * dim + j] = (i + j) % count;
                free (intPtr);
                return 0;       
}