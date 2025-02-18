typedef struct
{
    char **pLines;
    int *nums;
} Lines_nums;

int get_number_string(Lines_nums *__dest);
void get_numbers(FILE *f, int *numbers_buffer, int line_count);
int get_file_line_count(FILE *f);
int get_char_count_in_line(FILE *f);
#pragma once
