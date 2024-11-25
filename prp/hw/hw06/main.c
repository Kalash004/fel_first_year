#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    size_t width;
    size_t height;
    int *array; // 1d array
} Matrix;

int main(void)
{
    Matrix *matrix_a = get_matrix_from_input();
    scanf("\n");
    // obtain sign
    char sign = get_sign_from_input();
    scanf("\n");
    Matrix *matrix_b = get_matrix_from_input();
    // handle matrix arithmetics

    // print matrix
}

Matrix *get_matrix_from_input()
{
    // obtain matrix_a size
    Matrix *temp = malloc(sizeof(Matrix) * 1);
    size_t width;
    size_t height;
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
    int res = scanf("%li %li", &temp_height, &temp_width);
    if (res != 2)
    {
        // TODO: Handle scanf error
        return -1;
    }
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
                scanf("%i", &temp); // if last in row dont scanf space
                scanf("\n"); // TODO: check for this 
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

char get_sign_from_input()
{
    
}
