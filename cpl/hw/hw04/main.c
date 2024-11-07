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
// --------------------- UTILS --------------------------------

// ----------------- Error handling -------------------------------------
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
    int code;
    char *message;
} SError;
typedef enum
{
    UNKNOWN_ERROR_CODE,
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
};
void handle_fatal_error(int code);
void handle_non_fatal_error(int code);
void print_error_message(int code);
void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size);
/// @brief Same as handle_non_fatal_error but exits the program at the end
/// @param code Error code number
void handle_fatal_error(int code)
{
    print_error_message(code);
    exit(code);
}
/// @brief Finds error code, and prints it
/// @param code Error code number
void handle_non_fatal_error(int code)
{
    print_error_message(code);
}
/// @brief Prints error message from char *error_messages[]
/// @param code Error code number
void print_error_message(int code)
{
    char buffer[100];
    get_error_code_to_message(code, &buffer[0], 100);
    printf("%s\n", buffer);
}
/// @brief Goes thru SError errors[] and finds error by code. If doesnt exist returns Unknown error code
/// @param code int: Error code number
/// @param buffer char[]: Buffer to place message to
/// @param buffer_size int: Size of the buffer
void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size)
{
    SError error;
    for (int i = 0; i < END; ++i)
    {
        error = errors[i];
        if (error.code == code)
        {
            snprintf(buffer, buffer_size, "%s", error.message);
            return;
        }
    }
    error = errors[UNKNOWN_ERROR_CODE];
    snprintf(buffer, buffer_size, error.message, code);
}
// ----------------- Error handling -------------------------------------


// ----------------- Main Program ---------------------------------------

#include <stdio.h>
#include <stdbool.h>

#define BAD_OPPERATION_CODE 2
#define BAD_OPPERAND_CODE 3


typedef struct{
    char *num_first_cell; // since one number can be from 0 - 9
    int size; // size under 100;
    bool is_negative;
} MyLargeNumber;

int get_number_length();

int main()
{
    // Get number length
    int num1_length = get_number_length();
    // Make array with length ^
    char num1_mass[num1_length];
    // Save to struct
    MyLargeNumber num1 = {.size = num1_length, .num_first_cell = num1_mass, .is_negative = false};
    // Read number into array
    read_number_into_struct(&num1);
    // TEST: print number 1;
    print_my_large_number(num1);
    // Read opperation
    char operation = get_operation();
    // Get number length
    // Make array with length ^
    // Read number into array
    // Save to struct
    // Do math <- heap alloc
    // Print result
    // Free memory
}

int get_number_length()
{
    int __dest;
    scanf("%i\n", &__dest);
    return __dest;
}

int read_number_into_struct(MyLargeNumber *__dest)
{
    for (int i = 0; i < __dest->size; ++i)
    {
        char digit = read_digit(__dest);
        if (digit == 11) // may lead to some errors ;-;
            __dest->is_negative = true;
        
        __dest->num_first_cell[i] = digit;
    }
}

char read_digit()
{
    char possible_num = fgetc(stdin);
    if (possible_num == '-')
        return 11; // means -

    int result = char_to_num(possible_num);
    if (result == -1)
        return BAD_OPPERAND_CODE; // TODO: Handle exception if num is not a number
    
    return result;
}

int char_to_num(char __source)
{
    if (__source < '0' || __source > '9')
        return -1; // char not a number;

    int num = __source - '0';
    return num;
}

char get_operation()
{
    char operation;
    scanf("%c", &operation);
    if (!is_char_in_array(operation, 4, (char[]){'-', '+', '*', '/'})) // TODO: check if needs char[4]
    {
        handle_fatal_error(BAD_OPPERATION_CODE);
    }
    char c = getchar();
    if (c != '\n')
        handle_fatal_error(BAD_OPPERATION_CODE);
    return operation;
}


void print_my_large_number(MyLargeNumber number) {
    for (int i = 0; i < number.size; ++i) {
        fprintf(stdout, "%i",number.num_first_cell[i]);
    }
}