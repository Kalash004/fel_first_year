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

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef struct
{
    void *first_cell;
    int array_length;
    int element_size;
} Array;

int is_char_in_array(char c, int arr_size, char arr[]);
/// @brief Shifts array from from_index to to_index /example { arr = [1, 2, 3, 4, 5] -> (0, 3, arr) -> [4,5] }/, also changes arrays length
/// @param to_index
/// @param from_index
/// @param arr
/// @return -1 if to_index > from_index - error / 0 if done
int shift_array_left_to_index_from_index(int to_index, int from_index, Array *arr);
int shift_int_array_left_to_index_from_index(int to_index, int from_index, Array *arr);

int is_char_in_array(char c, int arr_size, char arr[])
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (c == arr[i])
            return TRUE;
    }
    return FALSE;
}
int shift_array_left_to_index_from_index(int to_index, int from_index, Array *arr)
{
    // TODO: Add memory clean
    if (to_index >= from_index)
        return -1;

    int used = to_index;
    void *first_cell = arr->first_cell;
    int arr_last_index = arr->array_length - 1;
    int element_size = arr->element_size;
    int j = from_index;
    do
    {
        memcpy((char *)first_cell + element_size * used, (char *)first_cell + element_size * j, element_size); // first_cell[used] = first_cell[i]
        ++used;
        ++j;
    } while (j <= arr_last_index);

    arr->array_length = arr->array_length - (from_index - to_index);

    return 0;
}
int shift_int_array_left_to_index_from_index(int to_index, int from_index, Array *arr)
{
    // TODO: Add memory clean
    if (to_index >= from_index)
        return -1;

    int used = to_index;
    int *first_cell = arr->first_cell;
    int arr_last_index = arr->array_length - 1;
    int j = from_index;
    do
    {
        first_cell[used] = first_cell[j];
        ++used;
        ++j;
    } while (j <= arr_last_index);

    arr->array_length = arr->array_length - (from_index - to_index);

    return 0;
}

// --------------------- UTILS --------------------------------

#include <stdio.h>
#include <stdlib.h>

// ----------------- Error handling -------------------------------------
typedef struct
{
    int code;
    char *message;
} SError;
typedef enum
{
    UNKNOWN_ERROR_CODE,
    BAD_INPUT,
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
    {.code = 100, .message = "Error: Chybny vstup!"}};
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
    fprintf(stderr, "%s\n", buffer);
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

#define BAD_INPUT_ERROR 100
typedef struct
{
    long long int number;
    Array *factors;
} Number_factors;

void set_everything_to_true(int *t, int count, int start_from);

void do_cycle(Array *primes, int *stop_flag);

void eratosthenes_sieve(int *temp_mass, int count);

void prime_generator(Array *primes, int count);

void get_primes(int *temp, int count, Array *primes);

void print_int_range_arr(int from, int to, Array arr);

int get_next_input(long long int *p_current_num);

void get_number_factor(Number_factors *destination, Array *primes, long long int current_num);

int get_max_factor_id(Array *primes, long long int current_num);

int get_exponent_count(Array *arr, int exponent, int start_from);

int get_next_factor(Array *primes, long long int *target, int *loop);

void print_factors(Array *primes, long long int input);

int main(void)
{
    // Generate primes and save to an array
    int prime_numbers_arr_size = 100000;
    int prime_numbers[prime_numbers_arr_size];
    Array primes = {.first_cell = &prime_numbers, .array_length = prime_numbers_arr_size, .element_size = sizeof(int)};
    prime_generator(&primes, 1000000);
    // Generate primes and save to an array end

    int stop = FALSE;
    do
    {
        do_cycle(&primes, &stop);
    } while (stop == FALSE);
}

void do_cycle(Array *primes, int *stop_flag)
{
    long long int input;
    int read_error = get_next_input(&input);
    if (read_error == 1)
    {
        *stop_flag = TRUE;
        return;
    }

    if (read_error != 0)
    {
        handle_fatal_error(read_error);
    }

    if (input == 1)
    {
        printf("Prvociselny rozklad cisla 1 je:\n1\n");
        return;
    }

    if (input == 0)
    {
        *stop_flag = TRUE;
        return;
    }

    print_factors(primes, input);
}

void prime_generator(Array *primes, int count)
{
    int temp_mass[count];
    set_everything_to_true(temp_mass, count, 0);
    eratosthenes_sieve(temp_mass, count);
    get_primes(temp_mass, count, primes);
}

void set_everything_to_true(int *t, int count, int start_from)
{
    for (int i = start_from; i < count; ++i)
    {
        t[i] = TRUE;
    }
}

void print_int_range_arr(int from, int to, Array arr)
{
    if (to > arr.array_length)
        return; // TODO: Error code

    for (int i = from; i < to; ++i)
    {
        int *first_cell = arr.first_cell;
        printf("%i: %i \n", i, first_cell[i]);
    }
}

void eratosthenes_sieve(int *temp_mass, int count)
{
    temp_mass[0] = temp_mass[1] = FALSE;
    int prime = 2;
    while (prime * prime <= count)
    {
        if (temp_mass[prime])
        {
            for (int i = prime * prime; i < count + 1; i += prime)
            {
                temp_mass[i] = FALSE;
            }
        }
        ++prime;
    }
}

void get_primes(int *temp, int count, Array *primes)
{
    int *primes_arr_cells = (int *)primes->first_cell;
    int used = 0;
    for (int i = 0; i < count; ++i)
    {
        if (temp[i])
        {
            primes_arr_cells[used] = i;
            ++used;
        }
    }
    primes->array_length = used;
}

int get_next_input(long long int *p_current_num)
{
    long long int input;
    int res = scanf("%lli\n", &input);
    if (res == EOF)
        return 1;
    if (res != 1)
        return BAD_INPUT_ERROR;
    if (input < 0)
        return BAD_INPUT_ERROR;

    *p_current_num = input;
    return 0;
}

int get_next_factor(Array *primes, long long int *target, int *loop)
{
    int *arr_first_cell = (int *)primes->first_cell;

    if (*loop < 0)
        handle_fatal_error(555); // Shouldnt happen

    int i = *loop;

    int factor = arr_first_cell[i];
    if (*target % factor == 0)
    {
        *target = *target / factor;
        return factor;
    }
    ++*loop;
    return -1;
}

int get_max_factor_id(Array *primes, long long int target)
{
    int i = 0;
    int *prime_cell = (int *)primes->first_cell;
    if (target < prime_cell[0])
        return -1;

    for (; i < primes->array_length; ++i)
    {
        if (prime_cell[i] > target)
            break;
    }

    return i;
}

void print_factors(Array *primes, long long int input)
{
    printf("Prvociselny rozklad cisla %lli je:\n", input);
    int max_factor_id = get_max_factor_id(primes, input);
    int loop = 0;
    int exponent_count = 0;
    int last_factor = 0;
    int factor_id = 0;
    long long int target = input;
    do
    {
        if (factor_id == max_factor_id)
            break; // End of printing

        int factor = get_next_factor(primes, &target, &factor_id);

        if (factor == -1)
        {
            continue;
        }

        if (factor == 0)
            break; // End of printing

        if (last_factor == factor)
        {
            ++exponent_count;
            continue;
        }
        if (exponent_count > 0 && factor != last_factor)
        {
            printf("^%i", exponent_count + 1);
            exponent_count = 0;
        }

        if (loop > 0 && factor_id < max_factor_id && factor != last_factor)
            printf(" x ");

        printf("%i", factor);

        last_factor = factor;
        ++loop;
    } while (factor_id < max_factor_id);
    if (exponent_count > 0)
        printf("^%i", exponent_count + 1);
    printf("\n");
}

int get_exponent_count(Array *arr, int exponent, int start_from)
{
    int count = 0;
    for (int i = start_from; i < arr->array_length; ++i)
    {
        if (((int *)(arr->first_cell))[i] != exponent)
        {
            break;
        }
        ++count;
    }
    return count;
}
