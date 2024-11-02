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
    } while (j < arr_last_index);

    arr->array_length = arr->array_length - (from_index - to_index);

    return 0;
}

// --------------------- UTILS --------------------------------

void wrapper_eratosthenes_sieve(Array *primes_arr, int amount_to_generate);

void generate_numbers(Array *arr);

int get_next_num(int old_num);

void print_int_range_arr(int from, int to, Array arr);

void eratosthenes_sieve(Array *generated_nums, Array *primes);

void remove_divisible_by(int num, Array *arr);

// void set_divisible_by_to_zero(int num, Array *arr);

// void find_remove_zeroes(Array *arr);

// int find_zeroes_count(Array *arr);

// void find_zeroes(Array *arr, int *zero_ids, int zero_count);

// void remove_zeroes(Array *arr, int *zero_ids, int zero_count);

int main(void)
{
    int prime_numbers_arr_size = 5;
    int prime_numbers[prime_numbers_arr_size];
    Array primes = {.first_cell = &prime_numbers, .array_length = prime_numbers_arr_size, .element_size = sizeof(int)};
    wrapper_eratosthenes_sieve(&primes, 10);
    print_int_range_arr(0, primes.array_length, primes);
}

void wrapper_eratosthenes_sieve(Array *primes_arr, int amount_to_generate) {
    int generated[amount_to_generate];
    Array generated_numbers = {.array_length = amount_to_generate, .element_size = sizeof(int), .first_cell = generated};
    generate_numbers(&generated_numbers);
    eratosthenes_sieve(&generated_numbers, primes_arr);
}

void generate_numbers(Array *arr)
{
    int num = 1;
    int *first_cell = (int *)arr->first_cell;
    for (int i = 0; i < arr->array_length; ++i)
    {
        num = get_next_num(num);
        first_cell[i] = num;
    }
}

int get_next_num(int old_num)
{
    return ++old_num;
}

void print_int_range_arr(int from, int to, Array arr)
{
    if (to > arr.array_length)
        return; // TODO: Error code

    for (int i = from; i <= to; ++i)
    {
        int *first_cell = arr.first_cell;
        printf("%i: %i \n", i, first_cell[i]);
    }
}

void eratosthenes_sieve(Array *generated_nums, Array *primes)
{
    if (generated_nums->array_length - 1 < 1)
        return;                           // TODO: Error code
    if (((int *)generated_nums->first_cell)[0] != 2) // must be 2
        return;                           // TODO: Error code

    int *known_primes = (int *)primes->first_cell;
    int *first_cell = generated_nums->first_cell;
    known_primes[0] = first_cell[0];
    int i = 0;
    for (; i < generated_nums->array_length; ++i)
    {
        remove_divisible_by(known_primes[i], generated_nums);
        known_primes[i + 1] = ((int *)generated_nums->first_cell)[0];
    }
    primes->array_length = i + 1;
}

// [2, 3, 5, 7]
// [3, 5, 7, 9]

// void remove_divisible_by(int num, Array *arr)
// {
// remove_divisible_by(num, arr);
// set_divisible_by_to_zero(num, arr);
// find_remove_zeroes(arr);
// }

// void set_divisible_by_to_zero(int num, Array *arr)
// {
//     int *first_cell = arr->first_cell;
//     for (int i = 0; i < arr->array_length - 1; ++i)
//     {
//         if (first_cell[i] % num == 0)
//             first_cell[i] = 0;
//     }
// }

void remove_divisible_by(int num, Array *arr)
{
    int *first_cell = arr->first_cell;
    int i = arr->array_length - 1;
    do
    {
        int current_num = first_cell[i];
        if (current_num % num == 0)
        {
            if (i == arr->array_length - 1)
            {
                arr->array_length -= 1;
                --i;
                continue;
            }
            shift_array_left_to_index_from_index(i, i + 1, arr);
        }
        --i;
    } while (0 <= i);
}

// void find_remove_zeroes(Array *arr)
// {
//     // int zero_count = find_zeroes_count(arr);
//     // find_zeroes(arr, zero_ids, zero_count);
//     // remove_zeroes(arr, zero_ids, zero_count);
// }

// int find_zeroes_count(Array *arr)
// {
//     int count = 0;
//     int *first_cell = arr->first_cell;
//     for (int i = 0; i < arr->array_length - 1; ++i)
//     {
//         if (first_cell[i] == 0)
//             ++count;
//     }
//     return count;
// }

// void find_zeroes(Array *arr, int *zero_ids, int zero_count)
// {
//     int used = 0;
//     int *first_cell = arr->first_cell;
//     for (int i = 0; i < zero_count - 1; ++i)
//     {
//         if (first_cell[i] == 0)
//         {
//             zero_ids[used] = i;
//             ++used;
//         }
//     }
// }

// void remove_zeroes(Array *arr, int *zero_ids, int zero_count)
// {
//     for (int i = 0; i < zero_count; ++i)
//     {
//         shift_array_from_start_left(zero_ids[i], arr);
//     }
// }