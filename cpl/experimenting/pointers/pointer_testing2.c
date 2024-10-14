#include <stdio.h>

int printff(int *x) {
    printf("%d",*x);
    return 0;
}

int main(void)
{
    int x = 0;
    int *y = &x;
    printff(y); 
    return 0;
}