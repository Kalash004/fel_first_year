#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


char* read_char();
int strcomp(char *pStr1,char *pStr2);
int strsize(char *pstr);

int main(void) {
    char *my_str = "Anton";
    char *input = read_char();
    int res = strcomp(my_str, input);
    printf("%d", res);
    return 0;
}

char* read_char() {
    int allocated = 5;
    int used = -1;
    char *memory = malloc(allocated);
    char c;
    do {
        c = getchar();
        ++used;
        if (allocated - 2 == used) {
            allocated *= 2;
            memory = realloc(memory, allocated);
        }
        *(memory + used) = c;
    } while (c != '\n');
    *(memory + used + 1) = '\0';
    
    // char d = *(memory+0);
    // for (int i = 0; d != '\0'; i++){
    //     d = *(memory+i);
    //     putchar(d);
    // }
    // putchar('\n');
    char f = memory[used+1];
    return memory;
}

int strcomp(char *pStr1,char *pStr2) {
    int size = strsize(pStr2);
    if (strsize(pStr1) != size) 
        return 1;
    int i = 0;
    char c1;
    do {
        c1 = *(pStr1 + i);
        char c2 = *(pStr2 + i);
        if (c1 != c2)
            return 1;
        ++i;
    } while (c1 != '\0');
    return 0;
}

int strsize(char *pStr) {
    int i = 0;
    char c;
    do {
        c = *(pStr + i);
        ++i;
    } while (c != '\000');
    return i;
}
