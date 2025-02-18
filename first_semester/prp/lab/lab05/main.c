#include <stdio.h>


void section1(void) {
    int x = 10;
    int *px = &x;
    printf("px = %X\n", (long int)px);
    printf("*px = %d\n", *px);
}

void section2(void) {
    char c = 10;
    int x = 10;
    int* px = &x;
    char* pc = &c;
    printf("sizeof(c) = %ld\n", sizeof(c));
    printf("sizeof(x) = %ld\n", sizeof(x));
    printf("sizeof(px) = %ld\n", sizeof(px));
    printf("sizeof(pc) = %ld\n", sizeof(pc));
 
    printf("(pc - px) = %ld\n", (pc - (char*)px));
}

void section3(void) {
    int x;
    char *pcx = &x;
    pcx[0] = 'a';
    pcx[1] = 'b';
    pcx[2] = 'c';
    pcx[3] = '\0';
    printf("x = %d\n", x);
    printf("&x = %ld\n", &x);
    printf("&x = %s\n", &x);
}

int main(void) {
    section3();
}