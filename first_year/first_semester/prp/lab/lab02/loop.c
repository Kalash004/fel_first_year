#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    for (unsigned int i = 0;i<(unsigned int)argc;i++){
        char str[10];
        sprintf(str, "Loop # %d\n", i);
        printf("%s",str);
    }
}