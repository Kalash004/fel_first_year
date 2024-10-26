#include <stdio.h>
#include <stdlib.h>

#define OK 0
// --------------------- UTILS --------------------------------
#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

int is_char_in_array(char c, int arr_size, char arr[])
{
    for (int i = 0; i < arr_size; ++i)
    {
        if (c == arr[i])
            return TRUE;
    }
    return FALSE;
}
// --------------------- UTILS --------------------------------
// ----------------- Error handling -------------------------------------
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
// ----------------- Program declarations -------------------------------
typedef struct
{
    char *word;
    int count;
} SWords_occurences;

int read_string_size(void);
char *read_string(int string_size);
int count_words(char *str);
char **prase_string_by_words(char *str, int word_count);
char *get_next_word(int *from_index, char *str);
int count_next_word_size(int from_index, char *str);
void print_words(char **pWords, int word_count);
void sort_words(char *words_array); // check qsort

int main(void)
{
    int str_size = read_string_size() + 1; // account for '\0'
    char *buffer = read_string(str_size);
    int word_count = count_words(buffer);
    char **words_buffer = prase_string_by_words(buffer, word_count);
    print_words(words_buffer, word_count);
    free(buffer);
    return OK;
}

int read_string_size(void)
{
    // TODO: Check input for errors;
    int str_size = 0;
    scanf("%i\n", &str_size);
    return str_size;
}
char *read_string(int string_size)
{
    // TODO: Check input for errors;
    int buffer_size = string_size * sizeof(char);
    char *buffer = malloc(buffer_size);
    for (int i = 0; i < string_size - 1; ++i)
    {
        char c = (char)getchar();
        buffer[i] = c;
    }
    buffer[++string_size] = '\0';
    return buffer;
}
int count_words(char *str)
{
    int word_count = 0;
    char c;
    int i = 0;
    do
    {
        c = str[i];
        if (is_char_in_array(c, 2, (char[2]){' ', '\0'}))
            ++word_count;
        ++i;
    } while (c != '\0');
    return word_count;
}
char **prase_string_by_words(char *str, int word_count)
{ // <----
    int start_from = 0;
    char **words = (char **)malloc(word_count * sizeof(char *));
    for (int i = 0; i < word_count; ++i)
    {
        char *word = get_next_word(&start_from, str);
        ++start_from;
        *(words + i * sizeof(char *)) = word;
    }
    return words;
}
char *get_next_word(int *from_index, char *str)
{
    int next_word_size = count_next_word_size(*from_index, str);
    char *word = malloc(next_word_size * sizeof(char));
    for (int i = 0; i < next_word_size; ++i)
    {
        word[i] = str[*from_index];
        ++*from_index;
    }
    return word;
}
int count_next_word_size(int from_index, char *str)
{
    char c;
    int word_size = 0;
    do
    {
        c = str[from_index];
        ++from_index;
        ++word_size;
    } while (!is_char_in_array(c, 2, (char[2]){' ', '\0'}));
    return word_size - 1;
}
void print_words(char **pWords, int word_count)
{
    for (int word_index = 0; word_index < word_count; ++word_index)
    {
        printf("%s", *(pWords + sizeof(char *)));
    }
}
