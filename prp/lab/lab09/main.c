#include <stdio.h>

int main(void){
    revert();
}

void rec_print(int n) {
    if (n == 0) {
        return;
    }
    printf("Hello world!\n");
    rec_print(n - 1);
}

int my_rec_strlen(char *str, size_t index) {
    if (str[index] == '\0') {
        return index;
    }
    return my_rec_strlen(str, index+1);
}

void revert() {
    char c = getchar();
    if (c == '\n') {
        return;
    }
    revert();
    putchar(c);
    return;
}