#include <stdio.h>

void read_while_not_eof();

int main(void) {
    read_while_not_eof();
}

void read_while_not_eof() {
    int read = 0;
    int x = 0;
    int y = 0;
    scanf("%d %d",&x,&y);
    printf("\n%d %d\n", x, y);
    x = y;
    scanf("%d", &read);
    if (x != y) 
        printf("\n%d\n",read);
}
