// #include "./my_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



void test_regex();

unsigned long my_strstr_match(const char *str, const char *substr, char **begin);

long int foo(My_Regex **regex, const char *str, size_t regex_size);

long int is_c_in_regex(My_Regex **regex, const char c, size_t last_reg_id);

bool is_c_in_reg_group(My_Regex *reg, const char c);

My_Regex **load_patter_into_struct(const char *pattern);

My_Regex *handle_group(size_t pattern_index, const char *pattern);

size_t get_group_size(size_t pattern_index, const char *pattern);

size_t get_pattern_real_size(const char *pattern);

void print_regex(My_Regex **regex);

// void test_my_strcat();

// void test_my_strspn();
void test_my_strtok();

size_t get_first_non_delim_char_location(char *str, const char *delim, size_t delim_size);

size_t get_delim_size(const char *delim);
size_t find_next_delim_end(char *str, const char *delim, size_t *str_loc, size_t delim_size, bool *done_flag, bool *delim_found_flag);
char *my_strtok(char *str, const char *delim);
void fill_target_with_token(char *temp, size_t token_start, size_t token_end, char *str);

int main()
{
    // test_my_strcat();
    // test_my_strspn();
    // test_my_strtok();
    test_regex();
    }

// void test_my_strcat()
// {
//     const char *str1 = "Hello friend ";
//     const char *str2 = "my name is Tony";
//     char *dest = (char *)malloc(14 * sizeof(char));
//     char **pDest = &dest;
//     strcpy(*pDest, str1);
//     printf("%s\n", *pDest);
//     my_strcat(pDest, &str2);
//     printf("%s\n", *pDest);
// }

// void test_my_strspn()
// {
//     const char str1[] = "aaaBBBccc";
//     const char str2[] = "aB";
//     unsigned long my_size = my_strspn(str1, str2);
//     unsigned long not_my = strspn(str1, str2);
//     printf("My: %li, theirs: %li \n", my_size, not_my);
// }

// void test_my_strtok()
// {
//     char str[] = ",..,..David,..david blake,.. bladsadas";
//     const char delim[] = ",..";
//     char *token = my_strtok(str, delim);

//     while (token != NULL)
//     {
//         printf("Token: %s\n", token);
//         free(token);
//         token = my_strtok(NULL, delim); // Subsequent calls with NULL
//     }
//     free(token);
// }

void test_regex()
{
    const char *pattern = "test";
    const char *str = "";
    My_Regex **regex = load_patter_into_struct(pattern);
    char *begin;
    my_strstr_match(str, pattern, &begin);
    if (begin != NULL)
    {
        printf("%s\n", begin);
    }
}



void print_regex(My_Regex **regex)
{
    My_Regex reg;
    size_t i = 0;
    do
    {
        reg = *(regex[i]);
        if (!reg.is_end_cell)
        {
            if (reg.is_prohibition_group)
            {
                printf("[^%s]", reg.regex);
                ++i;
                continue;
            }
            if (reg.is_regex_group)
            {
                printf("[%s]", reg.regex);
                ++i;
                continue;
            }
            printf("%s", reg.regex);
        }
        ++i;
    } while (!reg.is_end_cell);
    fflush(stdout);
}
