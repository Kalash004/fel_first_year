#include <stdio.h>

int get_number_length(int *__dest);

int main()
{
}

int get_number_length(int *__dest)
{
    scanf("%i\n", __dest);
}

int read_numbers(int *array, int size)
{
    for (int i = 0; i < size; ++i)
    {
        int num;
        array[i];
    }
}

int read_number(int *array, int size)
{
    char possible_num = fgetc(stdin);
    if (possible_num == '-')
        return -1; // means -

    int result = char_to_num(possible_num);
    if (result == -1)
        return NULL; // TODO: Handle exception if num is not a number
    
    return result;

}

int char_to_num(char __source)
{
    if (__source < '0' || __source > '9')
        return -1; // char not a number;

    int num = __source - '0';
    return num;
}