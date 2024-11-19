// --------------------- UTILS --------------------------------
#ifndef STDIOH
#define STDIOH
#include <stdio.h>
#endif

#ifndef STDLIBH
#define STDLIBH
#include <stdlib.h>
#endif

#ifndef STRINGH
#define STRINGH
#include <string.h>
#endif

#ifndef bool
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#endif

typedef struct
{
    void *first_cell;
    int array_length;
    int element_size;
} Array;

int is_char_in_array(char c, int arr_size, char arr[]);

int is_char_in_array(char c, int arr_size, char arr[])
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (c == arr[i])
            return TRUE;
    }
    return FALSE;
}
void shift_array_left(int shift_index, Array *arr);

void shift_array_left(int shift_index, Array *arr)
{
    int used = 0;
    void *first_cell = arr->first_cell;
    int arr_last_index = arr->array_length - 1;
    int element_size = arr->element_size;
    for (int i = 0; i < arr_last_index; ++i)
    {
        if (i < shift_index)
        {
            continue;
        }
        memcpy((char *)first_cell + element_size * used, (char *)first_cell + element_size * i, element_size); // 
        ++used;
    }
    arr->array_length = used + 1;
}

int get_min(int num1, int num2) {
    if (num1 > num2)
        return num1;
    return num2;
}
// --------------------- UTILS --------------------------------