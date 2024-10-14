#include <stdio.h>

int calc_dot_product(int *v1, int (*v2)[], int vec1_size, int vec2_size);

int main(void) {
    int v1[3] = {1,2,1};
    int v2[3][3];
    scanf("(%i,%i,%i,%i,%i,%i,%i,%i,%i)",&v2[0][0],&v2[0][1],&v2[0][2],&v2[1][0],&v2[1][1],&v2[1][2],&v2[2][0],&v2[2][1],&v2[2][2]);
    int prod[] = calc_dot_product(v1, v2, 3, 3);
    printf("Matrix multiplication: %i, %i, %i\n", prod[0], prod[1], prod[2]);
}

int calc_dot_product(int *v1, int (*v2)[], int vec1_size, int vec2_size) {
    int product[3] = {0,0,0};
    for (int i = 0; i < vec1_size; ++i) {
        for (int j = 0; j < vec2_size; ++j) {
            product[i] = product[i] + (v1[i] * v2[i][j]);
        }
    }
    return product;
}