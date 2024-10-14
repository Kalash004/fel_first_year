#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int read_input(int *ret_num_x, int *ret_num_y);
int count_sum(int num_x, int num_y);
int count_sub(int num_x, int num_y);
int count_mult(int num_x, int num_y);
int count_div(int num_x, int num_y);
double count_avg(int num_x, int num_y);
void convert_dec_to_hex(int num, char *pRet);
int reversed_hexa(int num, char *pRet);
void reverse(char *str);
int print_dec(int num_x, int num_y);
int print_hex(int num_x, int num_y);
int print_sum(int num_x, int num_y);
int print_sub(int num_x, int num_y);
int print_mult(int num_x, int num_y);
int print_div(int num_x, int num_y);
int print_avg(int num_x, int num_y);

int main(void)
{
    int num_x, num_y;
    int res = read_input(&num_x, &num_y);
    if (res != 0)
    {
        return res;
    }
    print_dec(num_x, num_y);
    print_hex(num_x, num_y);
    print_sum(num_x, num_y);
    print_sub(num_x, num_y);
    print_mult(num_x, num_y);
    int ret_div = print_div(num_x, num_y);
    print_avg(num_x, num_y);
    if (ret_div != 0)
    {
        return ret_div;
    }
    return 0;
}

int read_input(int *ret_num_x, int *ret_num_y)
{
    int num_x, num_y;
    int scn_res = scanf("%i %i", &num_x, &num_y);
    if (scn_res != 2)
    {
        fprintf(stderr, "Chyba nacitani vstupu!\n");
        return 100;
    }
    if (num_x >= 10001 || num_x <= -10001)
    {
        // return 100 and stderr "Vstup je mimo interval!"
        fprintf(stderr, "Error: Vstup je mimo interval!\n");
        return 100;
    }
    if (num_y >= 10001 || num_y <= -10001)
    {
        // return 100 and stderr "Vstup je mimo interval!"
        fprintf(stderr, "Error: Vstup je mimo interval!\n");
        return 100;
    }
    *ret_num_x = num_x;
    *ret_num_y = num_y;

    return 0;
}

int print_dec(int num_x, int num_y)
{
    printf("Desitkova soustava: %i %i\n", num_x, num_y);
    return 0;
}

int print_hex(int num_x, int num_y)
{
    // char ret_x[50];
    // char ret_y[50];
    // convert_dec_to_hex(num_x, ret_x);
    // convert_dec_to_hex(num_y, ret_y);
    printf("Sestnactkova soustava: %x %x\n", num_x, num_y);
    return 0;
}

int print_sum(int num_x, int num_y)
{
    int sum = count_sum(num_x, num_y);
    printf("Soucet: %i + %i = %i\n", num_x, num_y, sum);
    return 0;
}

int print_sub(int num_x, int num_y)
{
    int subtraction = count_sub(num_x, num_y);
    printf("Rozdil: %i - %i = %i\n", num_x, num_y, subtraction);
    return 0;
}

int print_mult(int num_x, int num_y)
{
    int multiplicated = count_mult(num_x, num_y);
    printf("Soucin: %i * %i = %i\n", num_x, num_y, multiplicated);
    return 0;
}

int print_div(int num_x, int num_y)
{
    if (num_y == 0)
    {
        printf("Podil: %i / %i = NaN\n", num_x, num_y);
        fprintf(stderr, "Error: Nedefinovany vysledek!\n");
        return 101;
    }
    int division = count_div(num_x, num_y);
    printf("Podil: %i / %i = %i\n", num_x, num_y, division);
    return 0;
}

int print_avg(int num_x, int num_y)
{
    double avg = count_avg(num_x, num_y);
    printf("Prumer: %.1f\n", avg);
    return 0;
}

int count_sum(int num_x, int num_y)
{
    return num_x + num_y;
}
int count_sub(int num_x, int num_y)
{
    return num_x - num_y;
}
int count_mult(int num_x, int num_y)
{
    return num_x * num_y;
}
int count_div(int num_x, int num_y)
{
    // if (num_x < num_y)
    // {
        // return 0;
    // }
    return num_x / num_y;
}
double count_avg(int num_x, int num_y)
{
    int sum = count_sum(num_x, num_y);
    return (double)sum / 2.0;
}

void convert_dec_to_hex(int num, char *pRet)
{
    int is_negative = 0;

    if (num < 0)
    {
        is_negative = 1;
        num = num * -1;
    }
    char ret[50];
    reversed_hexa(num, ret);
    if (is_negative)
    {
        strcat(ret, "-");
    }
    reverse(ret);
    strcpy(pRet, ret);
}

int reversed_hexa(int num, char *pRet)
{
    char ret[50] = "";
    int number = num * 16;
    int remainder;
    while ((number = number / 16) > 0)
    {
        char character;
        remainder = number % 16;
        if (remainder > 9)
        {
            switch (remainder)
            {
            case 10:
                character = 'a';
                break;
            case 11:
                character = 'b';
                break;
            case 12:
                character = 'c';
                break;
            case 13:
                character = 'd';
                break;
            case 14:
                character = 'e';
                break;
            case 15:
                character = 'f';
                break;
            default:
                break;
            }
            char temp[1] = {character};
            strcat(ret, temp);
        }
        else
        {
            char remainder_ch[10];
            sprintf(remainder_ch, "%i", remainder);
            strcat(ret, remainder_ch);
        }
    }
    strcpy(pRet, ret);
    return 0;
}

void reverse(char *str)
{
    int frst = 0;
    int last = strlen(str) - 1;
    char tmp;

    while (frst < last)
    {
        tmp = str[frst];
        str[frst] = str[last];
        str[last] = tmp;
        frst++;
        last--;
    }
}