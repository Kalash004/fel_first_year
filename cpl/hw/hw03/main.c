#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int compare_strings(const void *str1, const void *str2);
SWords_occurences **count_word_occurences(char **words, int word_count, int *ret_unique_word_count);
int get_unique_words_count(char **words, int word_count);
SWords_occurences **get_unique_words(char **words, int word_count);
void add_count(char *word, SWords_occurences **unique_words, int unique_words_count);
void print_word_and_occurences(SWords_occurences **words, int count);

SWords_occurences *create_object(char *word);

void free_memory(void *obtained_string, char **words_array, int word_count, SWords_occurences **occurences, int unique_words_count);

int main(void)
{
    int str_size = read_string_size() + 1; // account for '\0'
    char *obtained_string = read_string(str_size);
    int word_count = count_words(obtained_string);
    char **words_array = prase_string_by_words(obtained_string, word_count);
    qsort(words_array, word_count, sizeof(char *), compare_strings);
    int unique_words_count;
    SWords_occurences **occurences = count_word_occurences(words_array, word_count, &unique_words_count);
    print_word_and_occurences(occurences, unique_words_count);
    free_memory(obtained_string, words_array, word_count, occurences, unique_words_count);
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
        char c;
        scanf("%c", &c);
        buffer[i] = c;
    }
    buffer[string_size] = '\0';
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
        words[i] = word;
    }
    return words;
}
char *get_next_word(int *from_index, char *str)
{
    int next_word_size = count_next_word_size(*from_index, str) + 1;
    char *word = malloc(next_word_size * sizeof(char));
    for (int i = 0; i < next_word_size - 1; ++i)
    {
        word[i] = str[*from_index];
        ++*from_index;
    }
    word[next_word_size] = '\0';
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
        printf("\n%s\n", pWords[word_index]);
    }
}
int compare_strings(const void *obj1, const void *obj2)
{
    return strcmp(*(const char **)obj1, *(const char **)obj2);
}
SWords_occurences **count_word_occurences(char **words, int word_count, int *ret_unique_word_count)
{
    int unique_words_count = get_unique_words_count(words, word_count);
    SWords_occurences **unique_words = get_unique_words(words, word_count);
    for (int i = 0; i < word_count; ++i)
    {
        char *word1 = words[i];
        add_count(word1, unique_words, unique_words_count);
    }
    *ret_unique_word_count = unique_words_count;
    return unique_words;
}

int get_unique_words_count(char **words, int word_count)
{
    int unique_word_count = 0;
    for (int i = 0; i < word_count - 1; ++i)
    {
        char *word1 = words[i];
        char *word2 = words[i + 1];
        if (strcmp(word1, word2) != 0)
        {
            ++unique_word_count;
        }
    }
    ++unique_word_count; // to account for last word;
    return unique_word_count;
}
SWords_occurences **get_unique_words(char **words, int word_count)
{
    int unique_word_count = get_unique_words_count(words, word_count);
    SWords_occurences **unique_words = (SWords_occurences **)malloc(unique_word_count * sizeof(SWords_occurences *));
    int current_unique_word_index = 0;
    for (int i = 0; i < word_count - 1; ++i)
    {
        char *word1 = words[i];
        char *word2 = words[i + 1];
        if (strcmp(word1, word2) != 0)
        {
            unique_words[current_unique_word_index] = create_object(word1);
            ++current_unique_word_index;
        }
    }
    unique_words[current_unique_word_index] = create_object(words[word_count - 1]); // create entry for last word
    return unique_words;
}

void add_count(char *word, SWords_occurences **unique_words, int unique_words_count)
{
    for (int i = 0; i < unique_words_count; ++i)
    {
        SWords_occurences *pUnique_word = unique_words[i];
        if (strcmp(pUnique_word->word, word) == 0)
        {
            ++pUnique_word->count;
            return;
        }
    }
}
void print_word_and_occurences(SWords_occurences **words, int count)
{
    for (int i = 0; i < count; ++i)
    {
        SWords_occurences word = *(words[i]);
        printf("%i %s\n", word.count, word.word);
    }
}
SWords_occurences *create_object(char *word)
{
    char *unique_word = malloc((strlen(word) + 1) * sizeof(char));
    strcpy(unique_word, word);
    SWords_occurences *word_occurrence = malloc(sizeof(SWords_occurences));
    *word_occurrence = (SWords_occurences){.word = unique_word, .count = 0};
    return word_occurrence;
}

void free_memory(void *obtained_string, char **words_array, int word_count, SWords_occurences **occurences, int unique_words_count)
{
    free(obtained_string);
    for (int i = 0; i < word_count; ++i)
    {
        free(words_array[i]);
    }
    free(words_array);

    for (int i = 0; i < unique_words_count; ++i)
    {
        free(occurences[i]->word); // Free the word string
        free(occurences[i]);       // Free the SWords_occurences
    }
    free(occurences);
}
