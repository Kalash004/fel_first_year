// --------------------- Main program macros ------------------
#include <stdio.h>
#include <stdbool.h>

#define BAD_OPERATION_CODE 2
#define BAD_OPERAND_CODE 3
#define MALLOC_ERR_CODE 4

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
#include <stdbool.h>
#endif

typedef struct
{
    void *first_cell;
    int array_length;
    int element_size;
} Array;

bool is_char_in_array(char c, int arr_size, char arr[]);

bool is_char_in_array(char c, int arr_size, char arr[])
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (c == arr[i])
            return true;
    }
    return false;
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

int get_max(int num1, int num2);

int get_max(int num1, int num2)
{
    if (num1 > num2)
        return num1;
    return num2;
}

int get_min(int num1, int num2);

int get_min(int num1, int num2)
{
    if (num1 > num2)
        return num2;
    return num1;
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
#include <stdbool.h>
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

typedef struct
{
    int *num_first_cell; // since one number can be from 0 - 9
    int size;            // size under 100;
    bool is_negative;
} MyLargeNumber;

int get_number_length();

int read_number_into_struct(MyLargeNumber *__dest);

int read_digit();

int char_to_num(char __source);

char get_operation();

void print_my_large_number(MyLargeNumber number);

MyLargeNumber *do_math(MyLargeNumber num1, MyLargeNumber num2, char operation);

MyLargeNumber *do_sum(MyLargeNumber num1, MyLargeNumber num2);

void get_new_digit_sum(MyLargeNumber num1, MyLargeNumber num2, int *_new_digits, int distance_from_end, int _new_digit_possible_size, bool *is_overflow);

void check_set_overflow_sum(int *digit, bool *is_overflow);

MyLargeNumber *do_subtraction(MyLargeNumber num1, MyLargeNumber num2);

void get_new_digit_subtraction(MyLargeNumber num1, MyLargeNumber num2, int *_new_digits, int distance_from_end, int _new_digit_possible_size, bool *is_underflow);

MyLargeNumber *do_multiplication(MyLargeNumber num1, MyLargeNumber num2);

int *malloc_digit_array(int size);

MyLargeNumber *malloc_and_fill_MyLargeNumber(int *digits, int size);

int get_bigger_absolute_num_index(MyLargeNumber num1, MyLargeNumber num2);

void free_all(MyLargeNumber *_target);

bool get_largest_as_first_target(MyLargeNumber original_first, MyLargeNumber original_second, MyLargeNumber *target_first, MyLargeNumber *target_second);

int main()
{
    // Get number length
    int num1_length = get_number_length();
    // Make array with length ^
    int num1_mass[num1_length];
    // Save to struct
    MyLargeNumber num1 = {.size = num1_length, .num_first_cell = num1_mass, .is_negative = false};
    // Read number into array
    read_number_into_struct(&num1);
    // TEST: print number 1;

    // Read operation
    char operation = get_operation();

    int num2_length = get_number_length();
    int num2_mass[num2_length];
    MyLargeNumber num2 = {.size = num2_length, .num_first_cell = num2_mass, .is_negative = false};
    read_number_into_struct(&num2);

    // Do math <- heap alloc
    MyLargeNumber *answer = do_math(num1, num2, operation);
    // Print result
    print_my_large_number(*answer);
    // Free memory
    free_all(answer);
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
        int digit = read_digit(__dest);
        if (digit == 11) // may lead to some errors ;-;
            __dest->is_negative = true;

        __dest->num_first_cell[i] = digit;
    }
}

int read_digit()
{
    char possible_num = fgetc(stdin);
    if (possible_num == '-')
        return 11; // means -

    int result = char_to_num(possible_num);
    if (result == -1)
        return BAD_OPERAND_CODE; // TODO: Handle exception if num is not a number

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
    scanf("\n");
    char operation;
    scanf("%c", &operation);
    if (!is_char_in_array(operation, 4, (char[]){'-', '+', '*', '/'})) // TODO: check if needs char[4]
    {
        handle_fatal_error(BAD_OPERATION_CODE);
    }
    char c = getchar();
    if (c != '\n')
        handle_fatal_error(BAD_OPERATION_CODE);
    return operation;
}

void print_my_large_number(MyLargeNumber number)
{
    bool are_zeros_skipped = false;
    bool skipped_a_zero = false;
    if (number.is_negative)
        fprintf(stdout, "%c", '-');
    if (number.num_first_cell[0] != 0)
        are_zeros_skipped = true;
    for (int i = 0; i < number.size; ++i)
    {
        if (i + 1 == number.size)
        { // this is done to print even if the number is 0
            fprintf(stdout, "%i", number.num_first_cell[i]);
            continue;
        }
        if (!are_zeros_skipped && number.num_first_cell[i] == 0)
        {
            skipped_a_zero = true;
            continue;
        }
        if (!are_zeros_skipped && skipped_a_zero && number.num_first_cell[i] != 0)
        {
            are_zeros_skipped = true;
        }
        fprintf(stdout, "%i", number.num_first_cell[i]);
    }
    fprintf(stdout, "\n");
}

MyLargeNumber *do_math(MyLargeNumber num1, MyLargeNumber num2, char operation)
{
    MyLargeNumber *__answer;
    switch (operation)
    {
    case '+':
        __answer = do_sum(num1, num2);
        break;
    case '-':
        __answer = do_subtraction(num1, num2);
        break;
    case '*':
        __answer = do_multiplication(num1, num2);
        break;
    // case '/':
    //     __answer = do_division(num1, num2);
    //     break;
    default:
        // TODO: Handle exception
        break;
    }
    return __answer;
}

MyLargeNumber *do_sum(MyLargeNumber num1, MyLargeNumber num2)
{
    int _new_digit_possible_size = get_max(num1.size, num2.size) + 1;
    int *_new_digits = malloc_digit_array(_new_digit_possible_size);
    int _index_of_current_new_digit = 0;
    bool is_overflow = false;
    for (int i = 0; i < _new_digit_possible_size; ++i)
    {
        get_new_digit_sum(num1, num2, _new_digits, i, _new_digit_possible_size, &is_overflow);
        ++_index_of_current_new_digit;
    }
    return malloc_and_fill_MyLargeNumber(_new_digits, _new_digit_possible_size);
}
// TODO: operation wrapper

void get_new_digit_sum(MyLargeNumber num1, MyLargeNumber num2, int *_new_digits, int distance_from_end, int _new_digit_possible_size, bool *is_overflow)
{
    int new_digit = 0;
    if (*is_overflow)
    {
        new_digit = 1;
        *is_overflow = false;
    }
    if (num1.size > distance_from_end && num2.size > distance_from_end)
    { // Both numbers have next digit
        int digit_1 = num1.num_first_cell[num1.size - distance_from_end - 1];
        int digit_2 = num2.num_first_cell[num2.size - distance_from_end - 1];
        new_digit = digit_1 + digit_2 + new_digit;
        check_set_overflow_sum(&new_digit, is_overflow);
        _new_digits[_new_digit_possible_size - distance_from_end - 1] = new_digit;
        return;
    }
    if (num1.size > distance_from_end)
    { // Number 1 has digit, number 2 doesnt
        new_digit = num1.num_first_cell[num1.size - distance_from_end - 1] + new_digit;
        check_set_overflow_sum(&new_digit, is_overflow); // TODO: Check if this is even needed
        _new_digits[_new_digit_possible_size - distance_from_end - 1] = new_digit;
        return;
    }
    if (num2.size > distance_from_end)
    { // Number 2 has digit, number 1 doenst
        new_digit = num2.num_first_cell[num2.size - distance_from_end - 1] + new_digit;
        check_set_overflow_sum(&new_digit, is_overflow); // TODO: Check if this is even needed
        _new_digits[_new_digit_possible_size - distance_from_end - 1] = new_digit;
        return;
    }
    if (new_digit == 1)
    { // If overflown, add to last placec
        _new_digits[_new_digit_possible_size - distance_from_end - 1] = 1;
        return;
    }
    _new_digits[_new_digit_possible_size - distance_from_end - 1] = 0; // fill last place with 0
    return;
}

void check_set_overflow_sum(int *digit, bool *is_overflow)
{
    if (*digit > 9)
    {
        *is_overflow = true;
        *digit -= 10;
    }
}

MyLargeNumber *do_subtraction(MyLargeNumber num1, MyLargeNumber num2)
{
    int _new_digit_possible_size = get_max(num1.size, num2.size);
    int *_new_digits = malloc_digit_array(_new_digit_possible_size);
    bool are_nums_swapped = false;
    MyLargeNumber operational_num1;
    MyLargeNumber operational_num2;
    are_nums_swapped = get_largest_as_first_target(num1, num2, &operational_num1, &operational_num2);

    int _index_of_current_new_digit = 0;
    bool is_underflow = false;
    for (int i = 0; i < _new_digit_possible_size; ++i)
    {
        get_new_digit_subtraction(operational_num1, operational_num2, _new_digits, i, _new_digit_possible_size, &is_underflow);
        ++_index_of_current_new_digit;
    }
    MyLargeNumber *_temp = malloc_and_fill_MyLargeNumber(_new_digits, _new_digit_possible_size);
    if (are_nums_swapped) // take care of sign
        _temp->is_negative = true;
    return _temp;
}

void get_new_digit_subtraction(MyLargeNumber num1, MyLargeNumber num2, int *_new_digits, int distance_from_end, int _new_digit_possible_size, bool *is_underflow)
{
    int new_digit = 0;
    if (*is_underflow)
    {
        new_digit = 1;
        *is_underflow = false;
    }
    if (num1.size > distance_from_end && num2.size > distance_from_end)
    { // Both numbers have next digit
        int digit_1 = num1.num_first_cell[num1.size - distance_from_end - 1];
        int digit_2 = num2.num_first_cell[num2.size - distance_from_end - 1];
        new_digit = digit_1 - digit_2 - new_digit;
        check_set_overflow_sum(&new_digit, is_underflow);
        _new_digits[_new_digit_possible_size - distance_from_end - 1] = new_digit;
        return;
    }
    new_digit = num1.num_first_cell[num1.size - distance_from_end - 1] - new_digit;
    check_set_overflow_sum(&new_digit, is_underflow); // TODO: Check if this even needed
    _new_digits[_new_digit_possible_size - distance_from_end - 1] = new_digit;
    return;
}

void check_set_underflow(int *digit, bool *is_underflow)
{
    if (*digit < 0)
    {
        *is_underflow = true;
        *digit = 10 + digit;
    }
}

MyLargeNumber *do_multiplication(MyLargeNumber num1, MyLargeNumber num2)
{
    int _new_digit_possible_size = num1.size + num2.size;
    int *_new_digits = malloc_digit_array(_new_digit_possible_size);
    MyLargeNumber operational_num1;
    MyLargeNumber operational_num2;
    get_largest_as_first_target(num1, num1, &operational_num1, &operational_num2);
}

int *malloc_digit_array(int size)
{
    int *_new_digits = malloc(sizeof(int) * size);
    if (_new_digits == NULL)
    {
        handle_fatal_error(MALLOC_ERR_CODE);
    }
}

MyLargeNumber *malloc_and_fill_MyLargeNumber(int *digits, int size)
{
    MyLargeNumber *answer = (MyLargeNumber *)malloc(sizeof(MyLargeNumber));
    if (answer == NULL)
    {
        handle_fatal_error(MALLOC_ERR_CODE);
    }
    answer->num_first_cell = digits;
    answer->size = size; // might also contain 0 at the beginning of the array
    return answer;
}

/// @brief Get larger absolute number (not counting - and +)
/// @param num1 Number under index 1
/// @param num2 Number under index 2
/// @return Index of the bigger number
int get_bigger_absolute_num_index(MyLargeNumber num1, MyLargeNumber num2)
{
    if (num1.size == num2.size)
    {
        for (int i = 0; i < num1.size; ++i)
        {
            if (num1.num_first_cell[i] == num2.num_first_cell[i])
                continue;
            if (num1.num_first_cell[i] > num2.num_first_cell[i])
                return 1;
            return 2;
        }
        return 1; // Default
    }

    if (num1.size > num2.size)
        return 1;
    return 2;
}

void free_all(MyLargeNumber *_target)
{
    free(_target->num_first_cell);
    free(_target);
}

bool get_largest_as_first_target(MyLargeNumber original_first, MyLargeNumber original_second, MyLargeNumber *target_first, MyLargeNumber *target_second)
{
    if (get_bigger_absolute_num_index(original_first, original_second) == 1)
    {
        *target_first = original_first;
        *target_second = original_second;
        return false;
    }
    else
    {
        *target_first = original_second;
        *target_second = original_first;
        return true;
    }
}
