#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include "param_dealing.h"
// #include "file_handle.h"

typedef struct
{
    char **pLines;
    int *nums;
} Lines_nums;

int get_number_string(Lines_nums *__dest);
void get_numbers(FILE *f, int *numbers_buffer, int line_count);
int get_file_line_count(FILE *f);
int get_char_count_in_line(FILE *f);

int main(int argc, char *argv[])
{
    // deal_with_arguments(argc, argv);
    Lines_nums test;
    get_number_string(&test);
    for (int i = 0; i < 4; ++i)
    {
        printf("%i", test.nums[i]);
        fflush(stdout);
    }
}

int get_number_string(Lines_nums *__dest)
{
    FILE *number_file = fopen("numbers.txt", "r");
    FILE *order_file = fopen("orders.txt", "r");
    if (number_file == NULL || order_file == NULL)
        return -1;

    int numbers_count = get_file_line_count(number_file);
    int *numbers = malloc(numbers_count * sizeof(int));
    if (numbers == NULL)
        return -1;

    int str_lines_count = get_file_line_count(order_file);
    char **lines = malloc(str_lines_count * sizeof(char *));

    get_numbers(number_file, numbers, numbers_count);

    fclose(number_file);
    fclose(order_file);

    __dest->nums = numbers;
    __dest->pLines = NULL;
}

void get_strings(FILE *f)
{
}

void get_numbers(FILE *f, int *numbers_buffer, int line_count)
{
    for (int i = 0; i < line_count; ++i)
    {
        fscanf(f, "%i", &numbers_buffer[i]);
    }
}

int get_file_line_count(FILE *f)
{
    fseek(f, 0, SEEK_SET);
    char buffer[500];
    int count = 0;
    while (!feof(f))
    {
        fgets(buffer, 500, f);
        ++count;
    }
    fseek(f, 0, SEEK_SET);
    return count;
}

int get_char_count_in_line(FILE *f)
{
    long start_cursor = ftell(f);
    char buffer[500];
    if (fgets(buffer, 500, f) == NULL)
        return 0; // couldn't read
    int count = (int)strlen(buffer);
    fseek(f, -count, SEEK_CUR);
    return count;
}