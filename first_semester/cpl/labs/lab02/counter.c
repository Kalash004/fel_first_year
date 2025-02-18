#include <stdio.h>

int main(void){
    int sum = 0;
    int input = 1;
    while (input != 0) {
        int ret = scanf("%i",&input);
        if (ret != 1) {
            return 1;
        }
        sum = sum + input;
        if (sum > 25) {
            input = 0;
        }
    }
    printf("%i\n", sum);
    return 0;
}

// while (scanf() == 1 && input != 0)