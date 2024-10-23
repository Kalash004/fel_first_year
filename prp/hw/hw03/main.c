/**
 * Range : <3,69>
 * num1 = house width, num2 = house hight
 * if error during reading input - stderr"Error: Chybny vstup!" - return 100
 * if numbers not in range - stderr"Error: Vstup mimo interval!" - return 101
 * num1 must be non even - else: stderr"Error: Sirka neni liche cislo!" - return 102
 * Každý řádek končí znakem nového řádku, který je ihned za posledním symbolem na řádku ('X'), nebo ('|') pro volitelné zadání.
 */

#include <stdio.h>
#include <unistd.h>

typedef struct
{
    int width;
    int height;
    int fence_size;
} SHouse_dimensions;

#define OK 0
#define BAD_INPUT 100
#define MSG_BAD_INPUT "Error: Chybny vstup!"
#define INPUT_OUT_OF_BOUNDS 101
#define MSG_OUT_OF_BOUNDS "Error: Vstup mimo interval!"
#define WIDTH_IS_EVEN 102
#define MSG_WIDTH_IS_EVEN "Error: Sirka neni liche cislo!"
#define FENCE_SIZE_OUT_OF_BOUNDS 103
#define MSG_FENCE_SIZE_ERROR "Error: Neplatna velikost plotu!"

#define WALL_CHARACTER 'X'
#define EMPTY_CHARACTER ' '

#define TRUE 1
#define FALSE 0

int read_validate_input(SHouse_dimensions *house_dims);
int read_width_height(int *pWidth, int *pHeight);
int read_fence_size(int *pFence_size);
int validate_width_height(SHouse_dimensions house_dims);
int validate_fence_size(SHouse_dimensions house_dims);
void handle_error(int error_code);
void print_error_message(int error_code);
void draw_image(SHouse_dimensions house_dims);
void draw_roof(int width, char to_draw_roof_with);
void draw_mirror_parts_roof(int width, int middle_point_roof);
/**
 * Draws a line
 * _input:
 *      int width: width of the line
 *      unsigned int is_solid: bool(1 = True, 0 = False) if solid whole line is made of character
 */
void fill_line_solid(int width);
void fill_line_sides(int width, int current_row);
void fill_voidness_with_pattern(int count, int starting_pattern_index, char pattern[]);
void set_next_index_between(int *var, int min, int max);
void put_chars(int count, char char_to_put);
/**
 * Give error message for defined codes
 * _input:
 *      int error_code: error error_code
 *      char *buffer: buffer where to return the message
 *      unsigned int size: size of the buffer given
 */
void get_error_code_to_message(int error_code, char *buffer, unsigned int size);
/**
 * Prints message with new line at the end
 * _input:
 *      char *msg: message to print
 */
void print_with_newline(char *msg);

int main(void)
{
    SHouse_dimensions house_dims;
    int result = read_validate_input(&house_dims);
    if (result != OK)
    {
        handle_error(result);
        return result;
    }
    draw_image(house_dims);
}

int read_validate_input(SHouse_dimensions *house_dims)
{
    int width = 0;
    int height = 0;
    int status_read_width_height = read_width_height(&width, &height);
    if (status_read_width_height != OK)
        return status_read_width_height;
    int width_height_validation_result = validate_width_height(*house_dims);
    if (width_height_validation_result != OK)
        return width_height_validation_result;
    int fence_size = 0;
    int fence_size_validation_result = 0;
    if (read_fence_size(&fence_size))
        fence_size_validation_result = validate_fence_size(*house_dims);
    if (fence_size_validation_result != OK)
        return fence_size_validation_result;
    house_dims->width = width;
    house_dims->height = height;
    house_dims->fence_size = fence_size;
    return OK;
}

int validate_width_height(SHouse_dimensions house_dims)
{
    int width = house_dims.width;
    int height = house_dims.height;
    if ((width < 3 || width > 69) || (height < 3 || height > 69))
        return INPUT_OUT_OF_BOUNDS;
    if (width % 2 == 0)
    {
        printf("%d", width);
        return WIDTH_IS_EVEN;
    }
    return OK;
}

int read_width_height(int *pWidth, int *pHeight)
{
    int width;
    int height;
    int ret_scan = scanf("%d %d", &width, &height);
    if (ret_scan != 2)
        return BAD_INPUT;
    *pWidth = width;
    *pHeight = height;
    return OK;
}

void print_with_newline(char *msg)
{
    int str_elem_id = 0;

    while (*(msg + str_elem_id) != '\0')
    {
        putchar(msg[str_elem_id]);
        ++str_elem_id;
    }
    putchar('\n');
}

void get_error_code_to_message(int error_code, char *buffer, unsigned int size)
{
    switch (error_code)
    {
    case BAD_INPUT:
        snprintf(buffer, size, MSG_BAD_INPUT);
        break;
    case INPUT_OUT_OF_BOUNDS:
        snprintf(buffer, size, MSG_OUT_OF_BOUNDS);
        break;
    case WIDTH_IS_EVEN:
        snprintf(buffer, size, MSG_WIDTH_IS_EVEN);
        break;
    default:
        snprintf(buffer, size, "No error message for error code: %d\n", error_code);
    }
    return;
}

void draw_image(SHouse_dimensions house_dims)
{
    draw_roof(house_dims.width, WALL_CHARACTER);
    for (int i = 1; i <= house_dims.height; ++i)
    {
        if (i == 1 || i == house_dims.height)
        {
            fill_line_solid(house_dims.width);
            continue;
        }
        fill_line_sides(house_dims.width, i);
    }
    putchar('\n');
}

void draw_roof(int width, char to_draw_roof_with)
{
    int middle_point = (width / 2) + 1;
    put_chars(middle_point - 1, EMPTY_CHARACTER);
    put_chars(1, to_draw_roof_with);
    put_chars(middle_point - 1, EMPTY_CHARACTER);
    put_chars(1, '\n');
    draw_mirror_parts_roof(width, middle_point);
}

void draw_mirror_parts_roof(int width, int middle_point_roof)
{
    for (int i = middle_point_roof - 1; i >= 1; --i)
    {
        put_chars(i - 1, EMPTY_CHARACTER);
        put_chars(1, WALL_CHARACTER);
        put_chars(width - (2 * (i - 1)) - 2, EMPTY_CHARACTER);
        put_chars(1, WALL_CHARACTER);
        put_chars(i - 1, EMPTY_CHARACTER);
        put_chars(1, '\n');
    }
}

void fill_line_solid(int width)
{
    put_chars(width, WALL_CHARACTER);
    put_chars(1, '\n');
}

void fill_line_sides(int width, int current_row)
{
    int pattern_start;
    if (current_row % 2 == 0)
    {
        pattern_start = 1;
    }
    else
    {
        pattern_start = 0;
    }
    put_chars(1, WALL_CHARACTER);
    fill_voidness_with_pattern(width - 2, pattern_start, (char[2]){'o', '*'});
    put_chars(1, WALL_CHARACTER);
    put_chars(1, '\n');
}

void put_chars(int count, char char_to_put)
{
    if (count < 1)
        return;
    for (int i = 0; i < count; ++i)
    {
        putchar(char_to_put);
    }
}

void fill_voidness_with_pattern(int count, int starting_pattern_index, char pattern[])
{
    int pattern_index = starting_pattern_index;
    for (int i = 0; i < count; ++i)
    {
        put_chars(1, pattern[pattern_index]);
        set_next_index_between(&pattern_index, 0, 1);
    }
}

void set_next_index_between(int *var, int min, int max)
{
    if (*var == max)
    {
        *var = min;
        return;
    }
    ++*var;
    return;
}

void print_error_message(int error_code)
{
    char buffer[100];
    get_error_code_to_message(error_code, &buffer[0], 100);
    print_with_newline(buffer);
}

void handle_error(int error_code)
{
    print_error_message(error_code);
}

int read_fence_size(int *pFence_size)
{
    int ret_scan = scanf(" %d", pFence_size);
    if (ret_scan != 1)
        return BAD_INPUT;
    return OK;
}

int validate_fence_size(SHouse_dimensions house_dims)
{
    if (house_dims.fence_size > house_dims.height) 
        return FENCE_SIZE_OUT_OF_BOUNDS;
    if (house_dims.fence_size < 0) 
        return FENCE_SIZE_OUT_OF_BOUNDS;
}
