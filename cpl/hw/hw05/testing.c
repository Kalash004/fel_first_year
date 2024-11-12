// #include "./my_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    bool is_end_cell;
    bool is_prohibition_group;
    bool is_regex_group;
    char *regex;
} My_Regex;

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
    const char *pattern = "anton[k]";
    My_Regex **regex = load_patter_into_struct(pattern);
    print_regex(regex);
}

unsigned long my_strstr_match(const char *str, const char *substr, char **begin)
{
}

My_Regex **load_patter_into_struct(const char *pattern)
{
    size_t pattern_real_size = get_pattern_real_size(pattern);
    My_Regex **regex_array = malloc(sizeof(My_Regex *) * pattern_real_size + 1); // to account for arr_end

    size_t pattern_index = 0;
    size_t loop = 0;
    char c;
    do
    {
        My_Regex *reg;
        c = pattern[pattern_index];
        if (c == '\0')
            break;

        if (c == '[')
        {
            size_t group_length = get_group_size(pattern_index, pattern);
            My_Regex *reg = handle_group(pattern_index, pattern);
            if (reg == NULL)
                return NULL; // TODO: Handle null;
            pattern_index += group_length - 1;
        }
        else
        {
            char *pChar = malloc(sizeof(char) * 2);
            if (pChar == NULL)
                return NULL; // TODO: Handle null
            pChar[0] = c;
            pChar[1] = '\0';
            reg = malloc(sizeof(My_Regex) * 1);
            if (reg == NULL)
                return NULL; // TODO: Handle null
            reg->is_prohibition_group = false;
            reg->is_regex_group = false;
            reg->regex = pChar;
        }
        regex_array[loop] = reg;
        ++pattern_index;
        ++loop;
    } while (c != '\0');
    My_Regex *finish = malloc(sizeof(My_Regex) * 1);
    finish->is_end_cell = true;
    regex_array[loop] = finish;
    return regex_array;
}

My_Regex *handle_group(size_t pattern_index, const char *pattern)
{
    size_t group_size = get_group_size(pattern_index, pattern);
    char *pGroup = malloc(sizeof(char) * group_size - 1); // create space for group // -1 because we need '\0' at the end
    if (pGroup == NULL)
        return NULL; // TODO: Handle in upper function

    My_Regex *pRegex = malloc(sizeof(My_Regex));
    if (pRegex == NULL)
        return NULL; // TODO: Handle in upper function

    pRegex->regex = pGroup;
    pRegex->is_regex_group = true;
    pRegex->is_prohibition_group = false;

    ++pattern_index;
    char c = pattern[pattern_index];
    if (c == '^')
        pRegex->is_prohibition_group = true;

    size_t group_index = 0;
    for (; group_index < group_size - 2; ++pattern_index, ++group_index)
    {
        pGroup[group_index] = pattern[pattern_index];
    }
    pGroup[group_index + 1] = '\0';
    return pRegex;
}

size_t get_group_size(size_t pattern_index, const char *pattern)
{
    size_t size = 0;
    char c;
    do
    {
        c = pattern[pattern_index];
        ++pattern_index;
        ++size;
    } while (c != ']' && c != '\0');
    if (c == '\0')
        return 0; // not a group
    return size;
}

size_t get_pattern_real_size(const char *pattern)
{
    size_t current_location = 0;
    size_t real_size = 0;
    char c;
    do
    {
        c = pattern[current_location];
        if (c == '[')
        {
            size_t group_indicator_found_at = current_location;
            do
            {
                c = pattern[current_location];
                ++current_location;
            } while (c != ']' && c != '\0');
            if (c == '\0')
            {
                real_size += current_location - group_indicator_found_at; // deal with group start with no group end
                --real_size;                                              // dont count \0
                return real_size;
            }
            --current_location;
        }
        ++real_size;
        ++current_location;
    } while (c != '\0');
    --real_size; // count for \0
    return real_size;
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
            printf("%s", reg.regex);
        }
        ++i;
    } while (!reg.is_end_cell);
    fflush(stdout);
}

bool is_match(const char c, const char *regex, size_t regex_character_id)
{
}
