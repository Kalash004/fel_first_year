#include <stdio.h>

int main(int argc, char *argv[])
{
    int r = getchar();
    if (!(r <= '9' && r >= '0')) {
        return 1;
    }
    r = r - '0';
    for (int i = 0; i < r; ++i) {
        printf("%d: I like B0B36PRP!\n", i);
    }
}