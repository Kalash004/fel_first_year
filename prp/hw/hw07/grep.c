// --------------------------- Garbage collector --------------------------------

#ifndef GC
#define GC

#ifndef STDLIBH
#define STDLIBH
#include <stdlib.h>
#endif

#define MALLOCED_OJB_ARR_SIZE 1000

void **Objects_to_free;
size_t size_objects_to_free = 0;
size_t used_object_to_free_id = 0;

/// @brief Uses malloc to get pointer for heap, creates an entry into Objects_to_free and saves pointer there
/// @param size Size of object
/// @return Pointer
void *handled_malloc(size_t size);

/// @brief Looks for source in Objects_to_free to get the id, reallocs and saves new pointer to Objects_to_free
/// @param source Pointer to realloc
/// @param size Size of new malloc
/// @return New pointer
void *handled_realloc(void *source, size_t size);

/// @brief Finds entry in array
/// @param source
/// @return
size_t find_malloced_obj_id(void *source);

/// @brief Frees objects in Objects_to_free
void free_objects();

void *handled_malloc(size_t size)
{
    if (size_objects_to_free == 0)
    {
        size_objects_to_free = MALLOCED_OJB_ARR_SIZE;
        void **temp = malloc(sizeof(void *) * size_objects_to_free);
        if (temp == NULL)
        {
            return NULL;
        }
        Objects_to_free = temp;
    }
    if (size_objects_to_free < used_object_to_free_id + 2)
    {
        size_objects_to_free *= 2;
        void **temp = realloc(Objects_to_free, size_objects_to_free * sizeof(void *));
        if (temp == NULL)
        {
            return NULL;
        }
        Objects_to_free = temp;
    }
    void *temp = malloc(size);
    if (temp == NULL)
    {
        return NULL;
    }
    Objects_to_free[used_object_to_free_id] = temp;
    ++used_object_to_free_id;
    return temp;
}

void *handled_realloc(void *source, size_t size)
{
    size_t obj_id = find_malloced_obj_id(source);
    void *temp = realloc(source, size);
    if (temp == NULL)
    {
        free(source); // possible data loss
    }
    Objects_to_free[obj_id] = temp;
    return temp;
}

size_t find_malloced_obj_id(void *source)
{
    for (size_t i = 0; i < MALLOCED_OJB_ARR_SIZE; ++i)
    {
        if (source == Objects_to_free[i])
            return i;
    }
    return MALLOCED_OJB_ARR_SIZE; // TODO: check for problems
}

void free_objects()
{
    for (size_t i = 0; i < used_object_to_free_id; ++i)
    {
        free(Objects_to_free[i]);
    }
    free(Objects_to_free);
}
#endif

// --------------------------- Garbage collector --------------------------------

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
    BAD_REQUEST,
    NOT_IMPLEMENTED,
    MALLOC_FATAL,
    FILE_WASNT_OPENNED,
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
    {.code = 100, .message = "Error: Program was ran without needed parameters"},
    {.code = 2, .message = "Error: Not implemented"},
    {.code = 200, .message = "Fatal Error: Malloc didnt work"},
    {.code = 3, .message = "Fatal Error: Was not able to open file"},

};
void handle_fatal_error(int code);
void handle_non_fatal_error(int code);
void print_error_message(int code);
void get_error_code_to_message(int code, char buffer[], unsigned int buffer_size);
int get_error_output_code(int code);
/// @brief Same as handle_non_fatal_error but exits the program at the end
/// @param code Error code number
void handle_fatal_error(int code)
{
    print_error_message(code);
    int err_code = get_error_output_code(code);
    exit(err_code);
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
    if (code > END)
    {
        error = errors[UNKNOWN_ERROR_CODE];
        snprintf(buffer, buffer_size, error.message, code);
        return;
    }
    error = errors[code];
    snprintf(buffer, buffer_size, "%s", error.message);
    return;
}
int get_error_output_code(int code)
{
    SError error;
    if (code > END)
    {
        error = errors[UNKNOWN_ERROR_CODE];
        return error.code;
    }
    return errors[code].code;
}
// ----------------- Error handling -------------------------------------

int no_options_handler(char **argv);

int no_filepath_handler(char **argv);

size_t find_write_needed_lines(const char *pattern, char **lines, size_t lines_count);

int pattern_in_str(const char *pattern, char *str);

char **get_lines(FILE *f, size_t *line_count);

char *read_line(FILE *f);

char get_end_char_of_str(char *target);

int basic_handler(FILE *f, const char *pattern);

typedef struct
{
    
} Options;

int main(int argc, char **argv)
{
    // Parse arguments
    // find file
}

int no_options_handler(char **argv)
{
    // get pattern
    const char *pattern = argv[1];
    // get file path
    const char *file_path = argv[2];
    // open file
    FILE *f = fopen(file_path, "r");
    if (f == NULL)
        handle_fatal_error(FILE_WASNT_OPENNED);
    // save lines to array
    int res = basic_handler(f, pattern);
    fclose(f);
    free_objects();
    return res;
}

int no_filepath_handler(char **argv)
{
    const char *pattern = argv[1];
    int res = basic_handler(stdin, pattern);
    free_objects();
    return res;
}

size_t find_write_needed_lines(const char *pattern, char **lines, size_t lines_count)
{
    size_t found_count = 0;
    for (size_t i = 0; i < lines_count; ++i)
    {
        char *line = lines[i];
        if (!pattern_in_str(pattern, line))
            continue;
        printf("%s\n", line);
        ++found_count;
    }
    return found_count;
}

/// @brief Checks if pattern in string
/// @param pattern Pattern in question
/// @param str Target string
/// @return Non 0 if pattern in string
int pattern_in_str(const char *pattern, char *str)
{
    size_t i, j;

    for (i = 0; str[i] != '\0'; i++)
    {
        for (j = 0; pattern[j] != '\0'; j++)
        {
            if (str[i + j] != pattern[j])
            {
                break;
            }
        }
        if (pattern[j] == '\0')
        {
            return 1;
        }
    }
    return 0;
}

char **get_lines(FILE *f, size_t *line_count)
{
    long original_cur_position = ftell(f);
    fseek(f, 0, SEEK_SET); // set cursor to 0;

    size_t size = 10;
    size_t used = 0;
    char **lines = handled_malloc(sizeof(char *) * size);
    if (lines == NULL)
        handle_fatal_error(MALLOC_FATAL);

    while (!feof(f))
    {
        if (used == size - 2)
        {
            size *= 10;
            lines = handled_realloc(lines, sizeof(char *) * size);
        }
        char *line = read_line(f);
        lines[used] = line;
        ++used;
    }
    *line_count = used;
    fseek(f, original_cur_position, SEEK_SET);
    return lines;
}

char *read_line(FILE *f)
{
    size_t buff_size = 10;
    size_t used = 0;
    char *buffer = handled_malloc(buff_size * sizeof(char));
    char c;
    do
    {
        if (used == buff_size - 1)
        {
            buff_size += 10;
            buffer = handled_realloc(buffer, buff_size);
        }
        c = fgetc(f);
        if (c == EOF || c == '\n')
        {
            buffer[used] = '\0';
            break;
        }
        buffer[used] = c;
        ++used;
    } while (c != EOF && c != '\n');
    return buffer;
}

char get_end_char_of_str(char *target)
{
    char last_c;
    char c;
    size_t i = 0;
    do
    {
        if (i > 0)
            last_c = c;
        c = target[i];
        ++i;
    } while (c != '\0');
    return last_c;
}

int basic_handler(FILE *f, const char *pattern)
{
    size_t lines_count = 0;
    char **lines = get_lines(f, &lines_count);
    // go thru lines and find needed line
    // write needed line
    size_t found = find_write_needed_lines(pattern, lines, lines_count);
    if (found == 0)
    {
        return 1;
    }
    return 0;
}

// int options_handler(char **argv)
// {
//     handle_fatal_error(NOT_IMPLEMENTED);
// }