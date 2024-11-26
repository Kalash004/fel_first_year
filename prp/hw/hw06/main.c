#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    size_t width;
    size_t height;
    int *array; // 1d array
} Matrix;

Matrix *get_matrix_from_input();

int get_matrix_size(size_t *width, size_t *height);

void load_data_from_input_to_array(int *matrix_array, size_t height, size_t width);

void free_matrix(Matrix *__target);

char get_sign_from_input_remove_newlines();

Matrix *calculate_matrices(Matrix *matrix_a, Matrix *matrix_b, char sign);

void print_matrix(Matrix matrix);

Matrix *handle_multiplication(Matrix matrix_a, Matrix matrix_b);

Matrix *handle_addition(Matrix matrix_a, Matrix matrix_b);

Matrix *handle_subtraction(Matrix matrix_a, Matrix matrix_b);

int main(void)
{
    Matrix *matrix_a = get_matrix_from_input();
    // obtain sign
    char sign = get_sign_from_input_remove_newlines();
    Matrix *matrix_b = get_matrix_from_input();
    // handle matrix arithmetics
    Matrix *matrix_c = calculate_matrices(matrix_a, matrix_b, sign);
    // print matrix
    print_matrix(*matrix_a);
    print_matrix(*matrix_b);
}

Matrix *get_matrix_from_input()
{
    // obtain matrix_a size
    Matrix *temp = malloc(sizeof(Matrix) * 1);
    size_t width = 0;
    size_t height = 0;
    int read_result = get_matrix_size(&width, &height);
    if (!read_result)
    {
        // TODO: Handle scanf error
    }
    // create matrix_a array
    int *matrix_arr = malloc(sizeof(int) * (width * height));
    // obtain matrix_a data
    load_data_from_input_to_array(matrix_arr, height, width);
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
        // TODO: Handle scanf error
        return -1;
    }
    *height = temp_height;
    *width = temp_width;
    return 0;
}

// Possible exception if data is not the right size TODO: handle
void load_data_from_input_to_array(int *matrix_array, size_t height, size_t width)
{
    size_t matrix_arr_index = 0;
    for (size_t h = 0; h < height; ++h)
    {
        for (size_t w = 0; w < width; ++w)
        {
            int temp;
            if (w == width - 1)
            {
                scanf("%i", &temp);       // if last in row dont scanf space
                char garbage = getchar(); // TODO: check for this
                if (garbage != EOF && garbage != '\n')
                {
                    // TODO: handle
                    putchar(garbage);
                }
            }
            else
            {
                scanf("%i ", &temp); // otherwise do scan space
            }
            matrix_array[matrix_arr_index] = temp;
            ++matrix_arr_index;
        }
    }
}

void free_matrix(Matrix *__target)
{
    free(__target->array);
    free(__target);
}

char get_sign_from_input_remove_newlines()
{
    scanf("\n");
    char c = getchar();
    scanf("\n");
    return c;
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
    printf("Math");
    return NULL;
}

Matrix *handle_addition(Matrix matrix_a, Matrix matrix_b)
{
    printf("Math");
    return NULL;
}

Matrix *handle_subtraction(Matrix matrix_a, Matrix matrix_b)
{
    printf("Math");
    return NULL;
}
