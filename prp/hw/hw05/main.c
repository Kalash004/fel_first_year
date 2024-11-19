#define INPUT_ERR_CODE 100
#define INPUT_TEXTS_DIFFERENT_SIZE_CODE 101
#define MALLOC_ERROR 102
#define INITIAL_BUFFER_SIZE 10

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
    INPUT_ERR,
    INPUT_TEXT_DIFF_SIZE_ERR,
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
    {.code = INPUT_ERR_CODE, .message = "Error: Chybny vstup!"},
    {.code = INPUT_TEXTS_DIFFERENT_SIZE_CODE, .message = "Error: Chybna delka vstupu!"},
    {.code = MALLOC_ERROR, .message = "Error: Malloc error"}};
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
#include <ctype.h>

char *read_input_ciphered(size_t *length);

char *read_input_partial_deciphered(size_t *length);

char *read_unknown_length_input(size_t *length);

void *my_malloc(size_t size);

void *my_realloc(void *source, size_t size);

int validate_ciphered_partial(char *ciphered, char *deciphered);

int validate_text_is_abc(char *text);

int validate_same_length(size_t ciphered_len, size_t partial_len);

char *handle_decipher(char *ciphered, char *partial, size_t len);

size_t handle_shift_comparison(int shift, char *ciphered, char *partial, size_t str_len);

char *get_ciphered_after_shift(int shift, char *ciphered, size_t str_len);

size_t get_compare_rate(char *to_compare, char *partial);

int check_can_shift(int current_shift, char *ciphered, size_t str_len, int step);

int get_best_shift_handler(char *ciphered, char *partial, size_t len);

int get_best_shift_right(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift);

int get_best_shift_left(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift);

void get_best_shift(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift, int step);

int main(void)
{
    size_t ciphered_len = 0;
    char *ciphered = read_input_ciphered(&ciphered_len);
    size_t partial_len = 0;
    char *partial_deciphered = read_input_partial_deciphered(&partial_len);
    // check ciphered and partial_deciphered are using abcde...
    if (!validate_ciphered_partial(ciphered, partial_deciphered))
    {
        free(ciphered);
        free(partial_deciphered);
        handle_fatal_error(INPUT_ERR_CODE);
    }
    // check ciphered_len and partial_len are same
    if (!validate_same_length(ciphered_len, partial_len))
    {
        free(ciphered);
        free(partial_deciphered);
        handle_fatal_error(INPUT_TEXTS_DIFFERENT_SIZE_CODE);
    }

    // decipher
    char *truly_deciphered = handle_decipher(ciphered, partial_deciphered, ciphered_len);
    // print deciphered
    printf("%s\n", truly_deciphered);
    free(ciphered);
    free(partial_deciphered);
    free(truly_deciphered);
}

char *read_input_ciphered(size_t *length)
{
    return read_unknown_length_input(length);
}

char *read_input_partial_deciphered(size_t *length)
{
    return read_unknown_length_input(length);
}

char *read_unknown_length_input(size_t *length)
{
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t current_cell = 0;
    char *buffer = my_malloc(buffer_size * sizeof(char));
    char last_char;
    do
    {
        // get char
        last_char = getc(stdin);
        // check buffer is big enough
        if (current_cell > buffer_size - 1)
        {
            buffer_size *= 2;
            buffer = my_realloc(buffer, buffer_size);
        }
        buffer[current_cell] = last_char;
        ++current_cell;
    } while (last_char != '\n');
    if (current_cell == 0)
    {
        // TODO: handle no input
    }
    --current_cell;
    buffer[current_cell] = '\0'; // change \n for \0
    *length = current_cell;
    return buffer;
}

void *my_malloc(size_t size)
{
    void *temp = malloc(size);
    if (temp == NULL)
    {
        handle_fatal_error(MALLOC_ERROR);
    }
}

void *my_realloc(void *source, size_t size)
{
    void *temp = realloc(source, size);
    if (temp == NULL)
    {
        free(source); // possible data loss
        handle_fatal_error(MALLOC_ERROR);
    }
}

int validate_ciphered_partial(char *ciphered, char *deciphered)
{
    if (!validate_text_is_abc(ciphered))
        return 0;
    if (!validate_text_is_abc(deciphered))
        return 0;
    return 1;
}

int validate_text_is_abc(char *text)
{
    char c;
    size_t i = 0;
    do
    {
        c = text[i];
        if (c == '\0')
            break;
        ++i;
        if (!isalpha(c))
        {
            return 0;
        }
    } while (c != '\0');
    return 1;
}

int validate_same_length(size_t ciphered_len, size_t partial_len)
{
    if (ciphered_len != partial_len)
        return 0;
    return 1;
}

char *handle_decipher(char *ciphered, char *partial, size_t len)
{
    int shift = get_best_shift_handler(ciphered, partial, len);
    char *temp = get_ciphered_after_shift(shift, ciphered, len);
    return temp;
}

size_t handle_shift_comparison(int shift, char *ciphered, char *partial, size_t str_len)
{
    char *to_compare = get_ciphered_after_shift(shift, ciphered, str_len);
    size_t rate = get_compare_rate(to_compare, partial);
    free(to_compare);
    return rate;
}

char *get_ciphered_after_shift(int shift, char *ciphered, size_t str_len)
{
    char *temp = my_malloc(str_len * sizeof(char));
    char c;
    size_t i = 0;
    do
    {
        c = ciphered[i];
        if (c == '\0')
        {
            temp[i] = '\0';
            break;
        }
        c = ciphered[i] + shift;
        temp[i] = c;
        ++i;
    } while (c != '\0');
    return temp;
}

size_t get_compare_rate(char *to_compare, char *partial)
{
    char compare_c;
    char partial_c;
    size_t rate = 0;
    size_t i = 0;
    do
    {
        compare_c = to_compare[i];
        partial_c = partial[i];
        if ((compare_c != '\0' && partial_c != '\0') && (compare_c == partial_c))
            ++rate;
        ++i;
    } while (compare_c != '\0');
    return rate;
}

int check_can_shift(int current_shift, char *ciphered, size_t str_len, int step)
{
    char *temp = get_ciphered_after_shift(current_shift + step, ciphered, str_len);
    int res = validate_text_is_abc(temp);
    free(temp);
    return res;
}

int get_best_shift_handler(char *ciphered, char *partial, size_t len)
{
    size_t best_rate = 0;
    int best_shift = 0;
    get_best_shift_right(ciphered, partial, len, &best_rate, &best_shift);
    get_best_shift_left(ciphered, partial, len, &best_rate, &best_shift);
    return best_shift;
}

int get_best_shift_handler_v2(char *ciphered, char *partial, size_t len) {

}


int get_best_shift_right(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift)
{
    get_best_shift(ciphered, partial, len, best_rate, best_shift, +1);
}

int get_best_shift_left(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift)
{
    get_best_shift(ciphered, partial, len, best_rate, best_shift, -1);
}

void get_best_shift(char *ciphered, char *partial, size_t len, size_t *best_rate, int *best_shift, int step)
{
    int current_shift = 0;
    do
    {
        // check rate and save best rate
        size_t rate = handle_shift_comparison(current_shift, ciphered, partial, len);
        if (rate > *best_rate)
        {
            *best_rate = rate;
            *best_shift = current_shift;
        }
        // check if can shift again
        if (!check_can_shift(current_shift, ciphered, len, step))
            break;
        // change shift if can
        current_shift += step;
    } while (1);
}
