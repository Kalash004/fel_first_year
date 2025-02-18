#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

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
    BAD_INPUT_ERR_CODE,
    BAD_MALLOC_CODE,
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
    {.code = 100, .message = "Error: Chybny vstup!"},
    {.code = 10101, .message = "Error: Malloc didnt work as intended but program still continued"}};
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
#define BUFFER_INIT_SIZE 10
#define MALLOCED_OJB_ARR_SIZE 1000

void **Objects_to_free;
size_t size_objects_to_free = 0;
size_t used_object_to_free_id = 0;

typedef struct
{
    size_t width;
    size_t height;
    int *array; // 1d array
} Matrix;

typedef struct
{
    Matrix *m;
    char operation;
} Matrix_and_operation;

int handle_multiple_matrices(); // used

Matrix multiply_until_hit_other_operation(size_t *i, Matrix_and_operation *operations, size_t length); // used

Matrix_and_operation get_next_matrix_and_operation(size_t i, Matrix_and_operation *arr); // used

Matrix_and_operation *read_create_operation_buffer(size_t *len_target); // used

Matrix *get_matrix_from_input(); // used

int get_matrix_size(size_t *width, size_t *height); // used

int load_data_from_input_to_array(int *matrix_array, size_t height, size_t width); // used

int get_next_int_from_str(char *str, int restart); // used

int contains_non_numbers(char *str); // used

char *read_line(); // used

char get_sign_from_input_remove_newlines(); // used

Matrix *calculate_matrices(Matrix *matrix_a, Matrix *matrix_b, char sign); // used

void print_matrix(Matrix matrix); // used

Matrix *handle_multiplication(Matrix matrix_a, Matrix matrix_b); // used

void multiply_row_col(Matrix *target, size_t row_a, size_t col_b, Matrix source_a, Matrix source_b); // used

Matrix *handle_addition(Matrix matrix_a, Matrix matrix_b); // used

Matrix *handle_subtraction(Matrix matrix_a, Matrix matrix_b); // used

int get_matrix_cell_value(Matrix _target, size_t height, size_t width); // used

void set_matrix_cell_value(Matrix *_target, size_t height, size_t width, int value); // used

Matrix *create_matrix(size_t height, size_t width); // used

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

int main(void)
{
    return handle_multiple_matrices();
}

int handle_multiple_matrices()
{
    size_t buffer_length = 0;
    Matrix_and_operation *operations = read_create_operation_buffer(&buffer_length);
    if (operations == NULL)
    {
        free_objects();
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    Matrix result = *operations[0].m;
    char current_operation = operations[0].operation;
    for (size_t i = 0; i < buffer_length - 1; ++i)
    {
        Matrix_and_operation next = get_next_matrix_and_operation(i, operations);
        if (next.operation == '*')
        {
            size_t skip = i;
            Matrix multiplication_result = multiply_until_hit_other_operation(&skip, operations, buffer_length);
            Matrix *temp = calculate_matrices(&result, &multiplication_result, current_operation);
            // printf("\nOther_res:\n");
            // print_matrix(*temp);
            result = *temp;
            i = skip;
            current_operation = operations[i].operation;
            --i;
        }
        else
        { // other operations
            Matrix *temp = calculate_matrices(&result, next.m, current_operation);
            result = *temp;
            current_operation = next.operation;
        }
    }
    print_matrix(result);
    free_objects();
    return 0;
}

Matrix multiply_until_hit_other_operation(size_t *i, Matrix_and_operation *operations, size_t length)
{
    ++*i; // move to the next cell
    Matrix result = *operations[*i].m;
    char operation = operations[*i].operation;
    while (operation == '*' && *i < length - 1)
    {
        Matrix_and_operation next = get_next_matrix_and_operation(*i, operations);
        result = *handle_multiplication(result, *next.m);
        operation = next.operation;
        ++*i;
    }
    return result;
}

Matrix_and_operation get_next_matrix_and_operation(size_t i, Matrix_and_operation *arr)
{
    return arr[i + 1];
}

Matrix_and_operation *read_create_operation_buffer(size_t *len_target)
{
    size_t current_size = BUFFER_INIT_SIZE;
    size_t used = 0;
    Matrix_and_operation *action_buffer = handled_malloc(current_size * sizeof(Matrix_and_operation));

    while (1)
    {
        if (used == current_size - 2)
        {
            current_size *= 2;
            action_buffer = handled_realloc(action_buffer, current_size * sizeof(Matrix_and_operation));
        }
        // obtain data
        Matrix *m = get_matrix_from_input();
        if (m == NULL)
        {
            return NULL;
        }
        char operation = get_sign_from_input_remove_newlines();
        if (operation == EOF)
            operation = ' ';
        action_buffer[used].m = m;
        action_buffer[used].operation = operation;
        ++used;
        if (operation == ' ')
        {
            break;
        }
    }
    *len_target = used;
    return action_buffer;
}

Matrix *get_matrix_from_input()
{
    // obtain matrix_a size
    Matrix *temp = handled_malloc(sizeof(Matrix) * 1);
    size_t width = 0;
    size_t height = 0;
    int read_result = get_matrix_size(&width, &height);
    scanf("\n");
    if (read_result)
    {
        return NULL;
    }
    // create matrix_a array
    int *matrix_arr = handled_malloc(sizeof(int) * (width * height));
    // obtain matrix_a data
    int data_read_result = load_data_from_input_to_array(matrix_arr, height, width);
    if (data_read_result)
    {
        return NULL;
    }
    temp->width = width;
    temp->height = height;
    temp->array = matrix_arr;
    return temp;
}

int get_matrix_size(size_t *width, size_t *height)
{
    size_t temp_width;
    size_t temp_height;
    int res = scanf("%lu %lu", &temp_height, &temp_width);
    if (res != 2)
    {
        return -1;
    }
    *height = temp_height;
    *width = temp_width;
    return 0;
}

int load_data_from_input_to_array(int *matrix_array, size_t height, size_t width)
{
    size_t matrix_arr_index = 0;
    for (size_t h = 0; h < height; ++h)
    {
        char *line = read_line();
        if (!contains_non_numbers(line))
        {
            return 1;
        }
        for (size_t w = 0; w < width; ++w)
        {
            int restart_flag = 0;
            if (w == 0)
                restart_flag = 1;
            int temp = get_next_int_from_str(line, restart_flag);
            matrix_array[matrix_arr_index] = temp;
            ++matrix_arr_index;
        }
    }
    return 0;
}

int get_next_int_from_str(char *str, int restart)
{
    static size_t id;
    if (restart)
    {
        id = 0;
    }
    char c;
    char number[11] = {0};
    size_t used = 0;
    do
    {
        c = str[id];
        if (c == '-' || isdigit(c))
        {
            number[used] = c;
            ++used;
            ++id;
            continue;
        }
        if (used > 0 && !isdigit(c))
            break;
        ++id;
    } while (c != '\0');
    int target;
    sscanf(number, "%i", &target);
    return target;
}

int contains_non_numbers(char *str) // flawed -
{
    size_t i = 0;
    char c;
    do
    {
        c = str[i];
        if (c != ' ' && c != '-' && c != '\0')
        {
            if (!isdigit(c))
                return 0;
        }
        ++i;
    } while (c != '\0');
    return 1;
}

char *read_line()
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
        c = getchar();
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

char get_sign_from_input_remove_newlines()
{
    char test_eof = getchar();
    if (test_eof != EOF)
        scanf("\n");
    return test_eof;
}

Matrix *calculate_matrices(Matrix *matrix_a, Matrix *matrix_b, char sign)
{
    switch (sign)
    {
    case '*':
        return handle_multiplication(*matrix_a, *matrix_b);
        break;
    case '+':
        return handle_addition(*matrix_a, *matrix_b);
        break;
    case '-':
        return handle_subtraction(*matrix_a, *matrix_b);
        break;
    default:
        return NULL;
        break;
    }
}

void print_matrix(Matrix matrix)
{
    printf("%li %li\n", matrix.height, matrix.width);
    size_t matrix_id = 0;
    for (size_t h = 0; h < matrix.height; ++h)
    {
        for (size_t w = 0; w < matrix.width; ++w)
        {
            if (w == matrix.width - 1)
            {
                printf("%i\n", matrix.array[matrix_id]);
            }
            else
            {
                printf("%i ", matrix.array[matrix_id]);
            }
            ++matrix_id;
        }
    }
}

Matrix *handle_multiplication(Matrix matrix_a, Matrix matrix_b)
{
    if (matrix_a.width != matrix_b.height)
    {
        free_objects();
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    Matrix *target = create_matrix(matrix_a.height, matrix_b.width);
    for (size_t row_a = 0; row_a < matrix_a.height; ++row_a)
    {
        for (size_t col_b = 0; col_b < matrix_b.width; ++col_b)
        {
            multiply_row_col(target, row_a, col_b, matrix_a, matrix_b);
        }
    }
    return target;
}

void multiply_row_col(Matrix *target, size_t row_a, size_t col_b, Matrix source_a, Matrix source_b)
{
    int result = 0;
    for (size_t width_a_height_b_index = 0; width_a_height_b_index < source_a.width; ++width_a_height_b_index)
    {
        int num_a = get_matrix_cell_value(source_a, row_a, width_a_height_b_index);
        int num_b = get_matrix_cell_value(source_b, width_a_height_b_index, col_b);
        result += num_a * num_b;
    }
    set_matrix_cell_value(target, row_a, col_b, result);
}

Matrix *handle_addition(Matrix matrix_a, Matrix matrix_b)
{
    if (matrix_a.width != matrix_b.width || matrix_a.height != matrix_b.height)
    {
        free_objects();
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    Matrix *target = create_matrix(matrix_a.height, matrix_a.width);
    for (size_t row = 0; row < matrix_a.height; ++row)
    {
        for (size_t col = 0; col < matrix_a.width; ++col)
        {
            int val_a = get_matrix_cell_value(matrix_a, row, col);
            int val_b = get_matrix_cell_value(matrix_b, row, col);
            int result = val_a + val_b;
            set_matrix_cell_value(target, row, col, result);
        }
    }
    return target;
}

Matrix *handle_subtraction(Matrix matrix_a, Matrix matrix_b) // Its not DRY (dont repeat yourself), but we can live with that
{
    if (matrix_a.width != matrix_b.width || matrix_a.height != matrix_b.height)
    {
        free_objects();
        handle_fatal_error(BAD_INPUT_ERR_CODE);
        return NULL;
    }
    Matrix *target = create_matrix(matrix_a.height, matrix_a.width);
    for (size_t row = 0; row < matrix_a.height; ++row)
    {
        for (size_t col = 0; col < matrix_a.width; ++col)
        {
            int val_a = get_matrix_cell_value(matrix_a, row, col);
            int val_b = get_matrix_cell_value(matrix_b, row, col);
            int result = val_a - val_b;
            set_matrix_cell_value(target, row, col, result);
        }
    }
    return target;
}

int get_matrix_cell_value(Matrix _target, size_t height, size_t width)
{
    return _target.array[height * _target.width + width];
}

void set_matrix_cell_value(Matrix *_target, size_t height, size_t width, int value)
{
    _target->array[height * _target->width + width] = value;
}

Matrix *create_matrix(size_t height, size_t width)
{
    Matrix *temp = handled_malloc(sizeof(Matrix) * 1);
    int *temp_matrix_array = handled_malloc(sizeof(int) * height * width);
    for (size_t foo = 0; foo < height * width; ++foo) // Set values of matrix to 0
    {
        temp_matrix_array[foo] = 0;
    }
    temp->array = temp_matrix_array;
    temp->height = height;
    temp->width = width;
    return temp;
}

void *handled_malloc(size_t size)
{
    if (size_objects_to_free == 0)
    {
        size_objects_to_free = MALLOCED_OJB_ARR_SIZE;
        void **temp = malloc(sizeof(void *) * size_objects_to_free);
        if (temp == NULL)
        {
            handle_non_fatal_error(BAD_MALLOC_CODE);
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
            handle_non_fatal_error(BAD_MALLOC_CODE);
            return NULL;
        }
        Objects_to_free = temp;
    }
    void *temp = malloc(size);
    if (temp == NULL)
    {
        handle_non_fatal_error(BAD_MALLOC_CODE);
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
        handle_non_fatal_error(BAD_MALLOC_CODE);
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
