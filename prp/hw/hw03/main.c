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
    int fence_start_index;
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

int main(void);

int read_validate_input(SHouse_dimensions *house_dims);
int read_validate_fence(SHouse_dimensions *house_dims);
int validate_width_height(SHouse_dimensions dimensions);
void count_fence_start_index(SHouse_dimensions *house_dims);
int read_width_height(SHouse_dimensions *dimensions, int *pB_is_fence_size_coming);
int validate_fence_size(SHouse_dimensions house_dims);
void handle_error(int error_code);
int read_fence_size(SHouse_dimensions *dimensions);
void get_print_error_message(int error_code);
void draw_image_with_fence(SHouse_dimensions house_dims);
void draw_roof(int width, char to_draw_roof_with);
void draw_mirror_parts_roof(int width, int middle_point_roof);
/**
 * Draws a line
 * _input:
 *      int width: width of the line
 *      unsigned int is_solid: bool(1 = True, 0 = False) if solid whole line is made of character
 */
void fill_middle(SHouse_dimensions house_dims);
void fill_middle_empty(SHouse_dimensions house_dims);
void fill_line_solid(SHouse_dimensions house_dim);
void fill_line_solid_end(SHouse_dimensions house_dim);
void fill_line_sides(SHouse_dimensions house_dims, int current_row);
void fill_line_sides_empty(SHouse_dimensions house_dims);
void place_fence_sides(SHouse_dimensions dimensions);
void place_fence_mid(SHouse_dimensions dimensions);
void print_pattern(int count, char *pattern, int size);
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
void draw_image(SHouse_dimensions house_dims);
/**
 * Prints message with new line at the end
 * _input:
 *      char *msg: message to print
 */
void print_with_newline(char *msg);

void print_error_message(char *msg);

int main(void)
{
    SHouse_dimensions house_dims;
    house_dims.fence_size = 0;
    int result = read_validate_input(&house_dims);
    if (result != OK)
    {
        handle_error(result);
        return result;
    }
    if (house_dims.fence_size != 0)
    {
        draw_image_with_fence(house_dims);
    }
    else
    {
        draw_image(house_dims);
    }
}

int read_validate_input(SHouse_dimensions *house_dims)
{
    int is_fence_size_coming = TRUE;
    int status_read_width_height = read_width_height(house_dims, &is_fence_size_coming);
    if (status_read_width_height != OK)
        return status_read_width_height;
    int width_height_validation_result = validate_width_height(*house_dims);
    if (width_height_validation_result != OK)
        return width_height_validation_result;
    if (!is_fence_size_coming)
        return OK;
    int ret_fence_validation = read_validate_fence(house_dims);
    if (ret_fence_validation != OK)
        return ret_fence_validation;
    return OK;
}

int read_validate_fence(SHouse_dimensions *house_dims)
{
    int fence_size_validation_result = 0;
    read_fence_size(house_dims);
    fence_size_validation_result = validate_fence_size(*house_dims);
    if (fence_size_validation_result != OK)
        return fence_size_validation_result;
    if (house_dims->fence_size != 0)
        count_fence_start_index(house_dims);
    return OK;
}

int validate_width_height(SHouse_dimensions dimensions)
{
    if ((dimensions.width < 3 || dimensions.width > 69) || (dimensions.height < 3 || dimensions.height > 69))
        return INPUT_OUT_OF_BOUNDS;
    if (dimensions.width % 2 == 0)
    {
        return WIDTH_IS_EVEN;
    }
    return OK;
}

void count_fence_start_index(SHouse_dimensions *house_dims)
{
    house_dims->fence_start_index = house_dims->height - house_dims->fence_size;
}

int read_width_height(SHouse_dimensions *dimensions, int *pB_is_fence_size_coming)
{
    int width;
    int height;
    int ret_scan = scanf("%d %d", &width, &height);
    if (ret_scan != 2)
        return BAD_INPUT;
    if (getchar() == '\n')
        *pB_is_fence_size_coming = FALSE;
    dimensions->width = width;
    dimensions->height = height;
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

void print_error_message(char *msg)
{
    fprintf(stderr, "%s\n", msg);
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
    case FENCE_SIZE_OUT_OF_BOUNDS:
        snprintf(buffer, size, MSG_FENCE_SIZE_ERROR);
        break;
    default:
        snprintf(buffer, size, "No error message for error code: %d\n", error_code);
    }
    return;
}

void draw_image(SHouse_dimensions house_dims)
{
    draw_roof(house_dims.width, WALL_CHARACTER);
    fill_line_solid(house_dims);
    fill_middle_empty(house_dims);
    fill_line_solid(house_dims);
}

void draw_image_with_fence(SHouse_dimensions house_dims)
{
    draw_roof(house_dims.width, WALL_CHARACTER);
    fill_line_solid(house_dims);
    fill_middle(house_dims);
    fill_line_solid_end(house_dims);
}

void draw_roof(int width, char to_draw_roof_with)
{
    int middle_point = (width / 2) + 1;
    put_chars(middle_point - 1, EMPTY_CHARACTER);
    put_chars(1, to_draw_roof_with);
    put_chars(1, '\n');
    draw_mirror_parts_roof(width, middle_point);
}

void draw_mirror_parts_roof(int width, int middle_point_roof)
{
    for (int i = middle_point_roof - 1; i > 1; --i)
    {
        put_chars(i - 1, EMPTY_CHARACTER);
        put_chars(1, WALL_CHARACTER);
        put_chars(width - (2 * (i - 1)) - 2, EMPTY_CHARACTER);
        put_chars(1, WALL_CHARACTER);
        put_chars(1, '\n');
    }
}

void fill_middle(SHouse_dimensions house_dims)
{
    for (int i = 2; i <= house_dims.height - 1; ++i)
    {
        fill_line_sides(house_dims, i);
    }
}

void fill_middle_empty(SHouse_dimensions house_dims)
{
    for (int i = 2; i <= house_dims.height - 1; ++i)
    {
        fill_line_sides_empty(house_dims);
    }
}

void fill_line_solid(SHouse_dimensions house_dim)
{
    put_chars(house_dim.width, WALL_CHARACTER);
    put_chars(1, '\n');
}

void fill_line_solid_end(SHouse_dimensions house_dim)
{
    put_chars(house_dim.width, WALL_CHARACTER);
    place_fence_sides(house_dim);
    put_chars(1, '\n');
}

void fill_line_sides(SHouse_dimensions house_dims, int current_row)
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
    fill_voidness_with_pattern(house_dims.width - 2, pattern_start, (char[2]){'*', 'o'});
    put_chars(1, WALL_CHARACTER);
    if (current_row == house_dims.fence_start_index + 1)
        place_fence_sides(house_dims);
    if (current_row > house_dims.fence_start_index + 1)
        place_fence_mid(house_dims);
    put_chars(1, '\n');
}

void fill_line_sides_empty(SHouse_dimensions house_dims)
{
    put_chars(1, WALL_CHARACTER);
    put_chars(house_dims.width - 2, EMPTY_CHARACTER);
    put_chars(1, WALL_CHARACTER);
    put_chars(1, '\n');
}

void place_fence_sides(SHouse_dimensions dimensions)
{
    char pattern[2] = {'-', '|'};
    print_pattern(dimensions.fence_size, pattern, 2);
}

void place_fence_mid(SHouse_dimensions dimensions)
{
    char pattern[2] = {' ', '|'};
    print_pattern(dimensions.fence_size, pattern, 2);
}

void print_pattern(int count, char *pattern, int size)
{
    int pattern_index = 0;
    for (int i = 0; i < count; ++i)
    {
        put_chars(1, pattern[pattern_index]);
        set_next_index_between(&pattern_index, 0, size - 1);
    }
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

void get_print_error_message(int error_code)
{
    char buffer[100];
    get_error_code_to_message(error_code, &buffer[0], 100);
    print_error_message(buffer);
}

void handle_error(int error_code)
{
    get_print_error_message(error_code);
}

int read_fence_size(SHouse_dimensions *dimensions)
{
    // if (getchar() == EOF)
    // {
        // dimensions->fence_size = 0;
        // return OK;
    // } 
    // TODO: fixme
    int ret_scan = scanf("%d", &dimensions->fence_size);
    if (ret_scan != 1)
        return BAD_INPUT;
    return OK;
}

int validate_fence_size(SHouse_dimensions house_dims)
{
    if (house_dims.fence_size == 0)
        return OK;
    if (house_dims.fence_size > house_dims.height - 1)
        return FENCE_SIZE_OUT_OF_BOUNDS;
    if (house_dims.fence_size < 0)
        return FENCE_SIZE_OUT_OF_BOUNDS;
    if (house_dims.fence_size % 2 != 0)
        return FENCE_SIZE_OUT_OF_BOUNDS;
    return OK;
}
