// #include "./my_str.h"
#include <stdio.h>
#include <stdlib.h>

// void test_my_strcat();

// void test_my_strspn();
void test_my_strtok();

size_t get_delim_size(const char *delim);
size_t find_next_delim_end(char *str, const char *delim, size_t *str_loc, size_t delim_size, int *done_flag);
char *my_strtok(char *str, const char *delim);
void fill_target_with_token(char *temp, size_t token_start, size_t token_end, char *str);


int main()
{
    // test_my_strcat();
    // test_my_strspn();
    test_my_strtok();
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

void test_my_strtok()
{
    char str[] = "10,20,30,40,50";
    const char delim[] = ",";
    char *token = my_strtok(str, delim);

    while (token != NULL)
    {
        printf("Token: %s\n", token);
        free(token);
        token = my_strtok(NULL, delim); // Subsequent calls with NULL
    }
    free(token);
}

