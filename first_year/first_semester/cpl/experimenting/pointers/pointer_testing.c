#include <stdio.h>

void foo(int *d) 
{
    *d = 1;
}

int main(void) 
{
    int d = 10;
    int *i = &d;
    foo(i);
    printf("%d\n", *i);
}