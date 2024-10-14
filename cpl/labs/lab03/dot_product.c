#include <stdio.h>

int calc_dot_product(int *v1, int *v2);

int main(void) {
    int v1[3] = {1,1,1};
    int v2[3];
    scanf("(%i,%i,%i)",&v2[0],&v2[1],&v2[2]);
    int prod = calc_dot_product(v1, v2);
    printf("Dot product: %i\n", prod);
}

int calc_dot_product(int *v1, int *v2) {
    int product = 0;
    for (int i = 0; i < 3; ++i) {
        product = product + (v1[i] * v2[i]);
    }
    return product;
}