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
#include <stdbool.h>

typedef struct
{
    int x;
    int y;
    bool is_last_cell;
} Vector2D;

char *read_input_ciphered(size_t *length);

char *read_input_partial_deciphered(size_t *length);

char *read_unknown_length_input(size_t *length);

void *my_malloc(size_t size);

void *my_realloc(void *source, size_t size);

int validate_ciphered_partial(char *ciphered, char *deciphered);

int validate_text_is_abc(char *text);

int validate_same_length(size_t ciphered_len, size_t partial_len);

char *handle_decipher(char *ciphered, char *partial, size_t len);

char *decipher(int shift, char *ciphered, size_t str_len);

int get_shift(char *ciphered, char *partial, size_t len);

int increase_count_of_occurrences(Vector2D *vecs, int shift);

void zero_out_vectors(Vector2D *vecs, size_t len);

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
    return temp;
}

void *my_realloc(void *source, size_t size)
{
    void *temp = realloc(source, size);
    if (temp == NULL)
    {
        free(source); // possible data loss
        handle_fatal_error(MALLOC_ERROR);
    }
    return temp;
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
    int shift = get_shift(ciphered, partial, len);
    char *temp = decipher(shift, ciphered, len);
    return temp;
}

char *decipher(int shift, char *ciphered, size_t str_len)
{   
    char alpha[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    char *temp = my_malloc(str_len * sizeof(char));
    char c;
    size_t i = 0;
    do
    {
        // Some fun ASCII table black sorcery - please let me live
        c = ciphered[i];
        if (c == '\0')
        {
            temp[i] = '\0';
            break;
        }
        if (shift > 10 && (c + shift > 'z'))
        {
            int offset = c + shift - 'z';
            c = offset + 'A' - 1 - 6; // 58 is offset to get to start of letters in ascii table
        }
        else if (shift < -10 && (c + shift > 'z')) {
            int offset = c + shift - 'z';
            c = offset + 'A' - 1 + 6; // 58 is offset to get to start of letters in ascii table
        }
        else if (c + shift > 'z') {
            int offset = c + shift - 'z';
            c = offset + 'A' - 1; // 58 is offset to get to start of letters in ascii table
        }
        else if (shift < -10 && (c + shift < 'A')) {
            int offset = c + shift - 'A';
            c = 'z' - offset;
        }
        else if (c <= 'Z' && (c + shift > 'Z') && (c + shift < 'a'))
        {
            c = c + shift + 6;
        }
        else if (c >= 'a' && (c + shift < 'a') && (c + shift > 'Z'))
        {
            c = c + shift - 6;
        }
        else if (c + shift < 'A')
        {
            c = c + shift + 58;
        }
        else
        {
            c = c + shift;
        }
        temp[i] = c;
        ++i;
    } while (c != '\0');
    return temp;
}

int get_shift(char *ciphered, char *partial, size_t len)
{
    char ciphered_c;
    char partial_c;
    size_t i = 0;
    Vector2D vecs[len + 1];
    vecs[len] = (Vector2D){.x = 0, .y = 0, .is_last_cell = true}; // make end element
    zero_out_vectors(vecs, len);
    size_t latest_vecs_index = 0;
    int shift = 0;
    // hashmap would be great for this
    do
    {
        ciphered_c = ciphered[i];
        partial_c = partial[i];
        shift = partial_c - ciphered_c;
        if (!increase_count_of_occurrences(vecs, shift))
        {
            Vector2D vec = {.x = shift, .y = 1, .is_last_cell = false}; // y = count of occurrences
            vecs[latest_vecs_index] = vec;
            ++latest_vecs_index;
        }
        ++i;
    } while (ciphered_c != '\0');
    Vector2D vec;
    int largest_occur = 0;
    int target_shift = 0;
    size_t j = 0;
    do
    {
        vec = vecs[j];
        if (vec.y > largest_occur)
        {
            largest_occur = vec.y;
            target_shift = vec.x;
        }
        ++j;
    } while (!vec.is_last_cell);
    return target_shift;
}

int increase_count_of_occurrences(Vector2D *vecs, int shift)
{
    Vector2D vec;
    size_t i = 0;
    do
    {
        vec = vecs[i];
        if (vec.x == shift && vec.y != -1)
        {
            vecs[i] = (Vector2D){.x = vec.x, .y = vec.y + 1, .is_last_cell = false};
            return 1;
        }
        ++i;
    } while (!vec.is_last_cell);
    return 0;
}

void zero_out_vectors(Vector2D *vecs, size_t len)
{
    for (size_t i = 0; i < len - 1; ++i)
    {

        vecs[i] = (Vector2D){.x = 0, .y = 0, .is_last_cell = false};
    }
    vecs[len - 1] = (Vector2D){.x = 0, .y = 0, .is_last_cell = true};
}
