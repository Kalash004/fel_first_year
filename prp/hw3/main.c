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

#define OK 0
#define BAD_INPUT 100
#define MSG_BAD_INPUT "Error: Chybny vstup!"
#define INPUT_OUT_OF_BOUNDS 101
#define MSG_OUT_OF_BOUNDS "Error: Vstup mimo interval!"
#define WIDTH_IS_EVEN 102
#define MSG_WIDTH_IS_EVEN "Error: Sirka neni liche cislo!"

#define WALL_CHARACTER 'X'
#define EMPTY_CHARACTER ' '

#define TRUE 1
#define FALSE 0

int read_validate_input(int *pWidth, int *pHeight);
int read_width_heigth(int *pWidth, int *pHeight);
int validate_input(int width, int height);
void handle_error(int error_code);
void print_error_message(int error_code);
void draw_image(int width, int height);
void draw_roof(int width, char to_draw_roof_with);
void draw_mirror_parts_roof(int width, int middle_point_roof);
/**
 * Draws a line
 * _input:
 *      int width: width of the line
 *      unsigned int is_solid: bool(1 = True, 0 = False) if solid whole line is made of character
 */
void fill_line_solid(int width);
void fill_line_sides(int width);
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
    int width = 0;
    int height = 0;
    int result = read_validate_input(&width, &height);
    if (result != OK)
    {
        handle_error(result);
        return result;
    }
    draw_image(width, height);
}

int read_validate_input(int *pWidth, int *pHeight)
{
    int width = 0;
    int height = 0;
    int status_read_width_height = read_width_heigth(&width, &height);
    if (status_read_width_height != OK)
        return status_read_width_height;
    int input_validation_result = validate_input(width, height);
    if (input_validation_result != OK)
        return input_validation_result;
    *pWidth = width;
    *pHeight = height;
    return OK;
}

int validate_input(int width, int height)
{
    if ((width < 3 || width > 69) || (height < 3 || height > 69))
        return INPUT_OUT_OF_BOUNDS;
    if (width % 2 == 0)
    {
        printf("%d", width);
        return WIDTH_IS_EVEN;
    }
    return OK;
}

int read_width_heigth(int *pWidth, int *pHeight)
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

void draw_image(int width, int height)
{
    draw_roof(width, WALL_CHARACTER);
    for (int i = 1; i <= height; ++i)
    {
        if (i == 1 || i == height)
        {
            fill_line_solid(width);
            continue;
        }
        fill_line_sides(width);
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

void fill_line_sides(int width)
{
    put_chars(1, WALL_CHARACTER);
    put_chars(width - 2, EMPTY_CHARACTER);
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
