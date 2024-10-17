#include <stdio.h>
#include <stdlib.h>

void print_fraction(int numerator, int denominator);
int gcd(int x, int y);
int read_input(int *ret_num_x, int *ret_num_y);

typedef struct simplified_fractions
{
    int numerator;
    int denominator;
} simpl_fract;

simpl_fract simplify(int x, int y, int gcd);

int main(void)
{
    int numerator, denominator;
    int ret = read_input(&numerator, &denominator);
    if (ret != 0)
    {
        return ret;
    }
    if (numerator == 0)
    {
        printf("%i\n", 0);
        return 0;
    }
    int greatest_common_divisor = gcd(numerator, denominator);
    simpl_fract fract = simplify(numerator, denominator, greatest_common_divisor);
    print_fraction(fract.numerator, fract.denominator);
    return 0;
}

int read_input(int *ret_num_x, int *ret_num_y)
{
    int num_x, num_y;
    int scn_res = scanf("%i / %i", &num_x, &num_y);
    if (scn_res != 2)
    {
        fprintf(stderr, "Error: Occured while reading input.\n");
        return 1;
    }
    if (num_y == 0)
    {
        fprintf(stderr, "Error: Cannot simplify an illegitimate fraction.\n");
        return 1;
    }
    *ret_num_x = num_x;
    *ret_num_y = num_y;

    return 0;
}

int gcd(int x, int y)
{
    int left = abs(x);
    int right = abs(y);
    while ((right != left))
    {
        if (right == 1 || left == 1)
        {
            return 1;
        }
        if (left > right)
        {
            left = left - right;
        }
        else
        {
            right = right - left;
        }
    }
    return left;
}

simpl_fract simplify(int x, int y, int gcd)
{
    simpl_fract simpl_fr = {x / gcd, y / gcd};
    return simpl_fr;
}

void print_fraction(int numerator, int denominator)
{
    printf("%i / %i\n", numerator, denominator);
}