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

void prime_gen(Array *primes, int count);

void set_everything_to_true(Array *arr, int start_from);

void print_int_range_arr(int from, int to, Array arr);

void eratosthenes_sieve(Array *primes, int count);

void get_primes(Array *temp, Array *primes);

int main(void)
{
    int prime_numbers_arr_size = 100000;
    int prime_numbers[prime_numbers_arr_size];
    Array primes = {.first_cell = &prime_numbers, .array_length = prime_numbers_arr_size, .element_size = sizeof(int)};
    prime_gen(&primes, 1000000);
    print_int_range_arr(primes.array_length - 10, primes.array_length, primes);
}

void prime_gen(Array *primes, int count)
{
    int temp_mass[count];
    Array temp = {.array_length = count, .element_size = sizeof(int), .first_cell = temp_mass};
    set_everything_to_true(&temp, 0);
    eratosthenes_sieve(&temp, 1000000);
    get_primes(&temp, primes);
}

void set_everything_to_true(Array *arr, int start_from)
{
    int *first_cell = arr->first_cell;
    for (int i = start_from; i < arr->array_length; ++i)
    {
        first_cell[i] = TRUE;
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

void eratosthenes_sieve(Array *primes, int count)
{
    int *known_primes = (int *)primes->first_cell;
    known_primes[0] = known_primes[1] = FALSE;
    int prime = 2;
    while (prime * prime <= count)
    {
        if (known_primes[prime])
        {
            for (int i = prime * prime; i < count + 1; i += prime)
            {
                known_primes[i] = FALSE;
            }
        }
        ++prime;
    }
}

void get_primes(Array *temp, Array *primes)
{
    int *temp_arr_cells = (int *)temp->first_cell;
    int *primes_arr_cells = (int *)primes->first_cell;
    int used = 0;
    for (int i = 0; i < temp->array_length; ++i)
    {
        if (temp_arr_cells[i])
        {
            primes_arr_cells[used] = i;
            ++used;
        }
    }
    primes->array_length = used;
}
