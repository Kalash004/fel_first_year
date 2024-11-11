#include <stdlib.h>

void my_strcat(char **_dest, const char **_source);
unsigned long my_strspn(const char *dest, const char *src);
/// Finds the next token in a byte string
/**

        - If such character was found, it is replaced by the null character '\0' and the pointer to the following character ??
          is stored in a static location for subsequent invocations. ??
      - The function then returns the pointer to the beginning of the token ??

*/
char *my_strtok(char *str, const char *delim);
#pragma once
