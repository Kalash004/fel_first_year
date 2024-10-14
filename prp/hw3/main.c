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


int read_validate_input(int *pWidth, int *pHeight);
int read_width_heigth(int *pWidth, int *pHeight);
void handle_error(int error_code);
void print_error_message(int error_code);
void draw_image(int width, int height);
/**
 * Draws a line
 * _input:
 *      int width: width of the line
 *      unsigned char is_solid: bool(0 = True, 1 = False) if solid whole line is made of character
 */
void draw_line(int width, unsigned char is_solid);
void fill_line(int width, char tofillwith, char *str);
void 

void print_array(char arr[], unsigned int size);
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
    int status_read_width_heigth = read_width_heigth(&width, &height);
    if (status_read_width_heigth != OK)
        return status_read_width_heigth;
    if ((width < 3 || width > 69) || (height < 3 || height > 69))
        return INPUT_OUT_OF_BOUNDS;
    if (width % 2 == 0)
    {
        printf("%d", width);
        return WIDTH_IS_EVEN;
    }
    *pWidth = width;
    *pHeight = height;
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
        snprintf(buffer, size, "No error error_code for error_code: %d\n", error_code);
    }
    return;
}

void draw_image(int width, int height)
{
    for (int i = 0; i < height; ++i)
    {
        if (i == 0 || i == height)
        {
            draw_line(width, width);
            continue;
        }
        draw_line(width, 2);
        sleep(1);
    }
}

void draw_line(int width, unsigned char is_solid)
{
    char line[width + 1];
    line[width] = '\0';
    if (is_solid)
        fill_line(width, 'X');
    else
        fill_line(1, width, ' ', &line[0]);
    print_array(line, width + 1);
}

void fill_line(unsigned int start_point, unsigned int end_point, char tofillwith, char *str)
{
    for (unsigned int i = start_point; i < end_point - 1; ++i)
    {
        *(str + i) = tofillwith;
    }
}

void print_array(char *arr, unsigned int size)
{
    for (unsigned int i = 0; i < size; ++i)
    {
        char to_print = *(arr+i);
        putchar(to_print);
    }
}

void print_error_message(int error_code) {
    char buffer[100];
    get_error_code_to_message(error_code, &buffer[0], 100);
    print_with_newline(buffer);
}

void handle_error(int error_code) {
    print_error_message(error_code);
}
