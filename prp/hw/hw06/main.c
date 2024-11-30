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
    END
} EError_codes;
SError errors[] = {
    {.code = -1, .message = "Error: Unknown error code: %d"},
    {.code = 100, .message = "Error: Chybny vstup!"},
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
#define BUFFER_INIT_SIZE 10

typedef struct
{
    size_t width;
    size_t height;
    int *array; // 1d array
} Matrix;

// typedef struct
// {
//     Identifiable_matrix *a;
//     Identifiable_matrix *b;
//     char operation;

// } Operation;

// typedef struct
// {
//     size_t matrix_a_id;
//     Matrix *pointer;
// } Identifiable_matrix;

typedef struct
{
    Matrix *m;
    char operation;
} Matrix_and_operation;

int handle_mandatory();

int handle_multiple_matrices();

Matrix multiply_until_hit_other_operation(size_t *i, Matrix_and_operation *operations, size_t length);

Matrix_and_operation get_next(size_t i, Matrix_and_operation *arr);

Matrix_and_operation *read_create_operation_buffer(size_t *len_target);

Matrix *get_matrix_from_input();

int get_matrix_size(size_t *width, size_t *height);

int load_data_from_input_to_array(int *matrix_array, size_t height, size_t width);

int get_next_int_from_str(char *str, int restart);

int contains_non_numbers(char *str);

char *read_line();

void free_matrix(Matrix *__target);

char get_sign_from_input_remove_newlines();

Matrix *calculate_matrices(Matrix *matrix_a, Matrix *matrix_b, char sign);

void print_matrix(Matrix matrix);

Matrix *handle_multiplication(Matrix matrix_a, Matrix matrix_b);

void multiply_row_col(Matrix *target, size_t row_a, size_t col_b, Matrix source_a, Matrix source_b);

Matrix *handle_addition(Matrix matrix_a, Matrix matrix_b);

Matrix *handle_subtraction(Matrix matrix_a, Matrix matrix_b);

int get_matrix_cell_value(Matrix _target, size_t height, size_t width);

void set_matrix_cell_value(Matrix *_target, size_t height, size_t width, int value);

Matrix *create_matrix(size_t height, size_t width);

int main(void)
{
    return handle_multiple_matrices();
}

int handle_mandatory()
{
    Matrix *matrix_a = get_matrix_from_input();
    if (matrix_a == NULL)
    {
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    // obtain sign
    char sign = get_sign_from_input_remove_newlines();
    Matrix *matrix_b = get_matrix_from_input();
    if (matrix_a == NULL)
    {
        free(matrix_a);
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    // handle matrix arithmetics
    Matrix *matrix_c = calculate_matrices(matrix_a, matrix_b, sign);
    // print matrix
    print_matrix(*matrix_c);
    // cleanup
    free_matrix(matrix_a);
    free_matrix(matrix_b);
    free_matrix(matrix_c);
    return 0;
}

int handle_multiple_matrices()
{
    size_t buffer_length = 0;
    Matrix_and_operation *operations = read_create_operation_buffer(&buffer_length);
    if (operations == NULL)
    {
        handle_fatal_error(BAD_INPUT_ERR_CODE);
    }
    Matrix result = *operations[0].m;
    char current_operation = operations[0].operation;
    for (size_t i = 0; i < buffer_length - 1; ++i)
    {
        Matrix_and_operation next = get_next(i, operations);
        if (next.operation == '*')
        {
            size_t skip = i;
            Matrix multiplication_result = multiply_until_hit_other_operation(&skip, operations, buffer_length);
            Matrix *temp = calculate_matrices(&result, &multiplication_result, current_operation);
            result = *temp;
            free(temp);
            i = skip;
            current_operation = operations[i].operation;
            --i;
        }
        else
        { // other operations
            Matrix *temp = calculate_matrices(&result, next.m, current_operation);
            free(result.array);
            result = *temp;
            free(temp);
            current_operation = next.operation;
        }
    }
    print_matrix(result);
    return 0;
}

Matrix multiply_until_hit_other_operation(size_t *i, Matrix_and_operation *operations, size_t length)
{
    ++*i; // move to the next cell
    Matrix result = *operations[*i].m;
    char operation = operations[*i].operation;
    while (operation == '*' && *i < length - 1)
    {
        Matrix_and_operation next = get_next(*i, operations);
        result = *handle_multiplication(result, *next.m);
        operation = next.operation;
        ++*i;
    }
    return result;
}

Matrix_and_operation get_next(size_t i, Matrix_and_operation *arr)
{
    return arr[i + 1];
}

Matrix_and_operation *read_create_operation_buffer(size_t *len_target)
{
    size_t current_size = BUFFER_INIT_SIZE;
    size_t used = 0;
    Matrix_and_operation *action_buffer = malloc(current_size * sizeof(Matrix_and_operation));

    while (1)
    {
        if (used == current_size)
        {
            // TODO: make safe realloc and malloc
            current_size *= current_size;
            action_buffer = realloc(action_buffer, current_size);
        }
        // obtain data
        Matrix *m = get_matrix_from_input();
        if (m == NULL)
        {
            for (int i = used; i >= 0; --i)
            {
                Matrix_and_operation temp = action_buffer[i];
                free(temp.m);
            }
            free(action_buffer);
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
    Matrix *temp = malloc(sizeof(Matrix) * 1);
    size_t width = 0;
    size_t height = 0;
    int read_result = get_matrix_size(&width, &height);
    scanf("\n");
    if (read_result)
    {
        free(temp);
        return NULL;
    }
    // create matrix_a array
    int *matrix_arr = malloc(sizeof(int) * (width * height));
    // obtain matrix_a data
    int data_read_result = load_data_from_input_to_array(matrix_arr, height, width);
    if (data_read_result)
    {
        free(matrix_arr);
        free(temp);
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
    char *buffer = malloc(buff_size * sizeof(char));

    char c;
    do
    {
        if (used == buff_size - 1)
        {
            buff_size += 10;
            buffer = realloc(buffer, buff_size);
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

void free_matrix(Matrix *__target)
{
    free(__target->array);
    free(__target);
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
        handle_fatal_error(BAD_INPUT_ERR_CODE);
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
    Matrix *temp = malloc(sizeof(Matrix) * 1);
    int *temp_matrix_array = malloc(sizeof(int) * height * width);
    for (size_t foo = 0; foo < height * width; ++foo) // Set values of matrix to 0
    {
        temp_matrix_array[foo] = 0;
    }
    temp->array = temp_matrix_array;
    temp->height = height;
    temp->width = width;
    return temp;
}
