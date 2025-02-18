#include <stdio.h>
#include <stdlib.h>

void increment(int *b) {
    int *a; 
    a = malloc(100000);
    a[10] = 11;
    printf("%i\n",a[10]);
    b = a;
}

int main(void) {
    int *b = 0;
    increment(b);
    printf("%i\n",b[10]);
    return 0;
}