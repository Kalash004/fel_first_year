// --------------------- UTILS --------------------------------
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

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
void shift_array_left(int shift_index, int arr_size, void *arr)
{
    int used = 0;
    for (int i = 0; i < arr_size; ++i)
    {
        if (i < shift_index)
        {
            continue;
        }
        char temp_char = (*buffer)[i];
        (*buffer)[used] = temp_char;
        ++used;
    }
    (*buffer)[used] = '\0';
}
// --------------------- UTILS --------------------------------
