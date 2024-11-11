// #include "./my_str.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void my_strcat(char **_dest, const char **_source);
size_t _get_str_size(const char *_source);


int main() {
    const char *str1 = "Hello friend ";
    const char *str2 = "my name is Tony";
    char *dest = (char *)malloc(14 * sizeof(char));
    char **pDest = &dest;
    strcpy(*pDest, str1);
    printf("%s\n", *pDest);
    my_strcat(pDest, &str2);
    printf("%s\n", *pDest);

}

void my_strcat(char **_dest, const char **_source){
    // find size of dest
    size_t _dest_size = _get_str_size(*_dest); 
    // find sie of source
    size_t _source_size = _get_str_size(*_source); 
    // realloc depending on size
    if (_dest_size - 1 + _source_size == _dest_size) // works only if source is 1 char and it is /n so no point in changing the str
        return; 
    char *_temp = (char *)realloc(*_dest, (_dest_size + _source_size - 1) * sizeof(char));
    if (_temp == NULL) {
        // TODO: define behavioure when realloc fails
        return;
    }
    size_t used = _dest_size - 1;
    for (int i = 0; i<_source_size; ++i){
        _temp[used] = (*_source)[i];
        ++used;
    }
    char **pTemp = malloc(sizeof(char *));
    pTemp = &_temp;
    _dest = pTemp;
}


size_t _get_str_size(const char *_source) {
    size_t str_size = 1;
    do {
        if (_source[str_size - 1] == '\0')
            break;
        ++str_size;
    } while (1);
    return str_size;
}