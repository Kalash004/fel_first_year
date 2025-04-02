#include "stdio.h"
#include "stdlib.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // open()
#include <sys/mman.h> // mmap(), munmap()
#include <sys/stat.h> // fstat()
#include <unistd.h>   // close()
#include <string.h>   // For memcpy()

#define OUTPUT_IMAGE_NAME "./testoutput.ppm"
#define P6_OFFSET 3
#define MAX_COLOR_OFFSET 4

char *parse_args_get_file_name(int argc, char *argv[]);
int get_width(void *mapped, int *next_offset);
int get_height(void *mapped, int offset, int *next_offset);
int get_line(void *source, char *target, int size);

void setPixel(unsigned char *image, int image_width, int x, int y, unsigned char red, unsigned char green, unsigned char blue);
void getPixel(unsigned char *image, int image_width, int x, int y, unsigned char *red, unsigned char *green, unsigned char *blue);

void convalute(unsigned char *image, int *mask, int width, int height, unsigned int *histogram);

int main(int argc, char *argv[])
{
    /*
    main :
    - konvoluce (dana maska)
    - histogram (stupne greyscale, intervaly <0;50>	<51;101> <102;152> <153;203> <204;255>)
    - prijmout jmeno filu
    - prijmout vstupni obrazek .ppm
    - zaostrit obrazek, savnout do output.ppm
    - histogram do output.txt
    sub-tasks :
    - optimalizace pameti
        (data a instrukce)
    - test s cachegrindem
    - casovac
    - SIMD instructions (single instruction multiple data)
    - optimalizace struktur
    */

    char *file_name = parse_args_get_file_name(argc, argv);
    if (!file_name)
    {
        return EXIT_FAILURE;
    }
    getchar();
    int file_descriptor = open(file_name, O_RDONLY);
    if (file_descriptor == -1)
    {
        printf("\nException during image file reading\n");
        return EXIT_FAILURE;
    }
    struct stat sb;
    if (fstat(file_descriptor, &sb) == -1)
    { // Get file size
        printf("\nException during fstat\n");
        return EXIT_FAILURE;
    }
    size_t filesize = sb.st_size;
    void *mapped_mem = mmap(NULL, filesize, PROT_READ, MAP_SHARED, file_descriptor, 0);
    if (mapped_mem == MAP_FAILED)
    {
        printf("\nException during mmap\n");
        return EXIT_FAILURE;
    }
    close(file_descriptor);
    getchar();

    int next_skip1 = 0;
    int next_skip2 = 0;
    int width = get_width(mapped_mem, &next_skip1);
    int height = get_height(mapped_mem, P6_OFFSET + next_skip1, &next_skip2);
    getchar();

    unsigned char *image = malloc((width * height * 3) * sizeof(unsigned char));
    size_t n = width * height * 3;
    memcpy(image, (mapped_mem + (P6_OFFSET + next_skip1 + next_skip2 + MAX_COLOR_OFFSET + 2)), n);
    getchar();

    unsigned int histogram[5];
    int mask[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    convalute(image, mask, width, height, histogram);
    printf("histogram : %u %u %u %u %u", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4]);
    getchar();

    FILE *save_to = fopen(OUTPUT_IMAGE_NAME, "wb");
    fprintf(save_to, "P6\n%i\n%i\n255\n", width, height);
    fwrite(image, sizeof(unsigned char), (width * height * 3), save_to);
    getchar();
    
    // free(mapped_mem);
    free(image);

}

int get_width(void *mapped, int *next_offset)
{
    char buffer[1024] = {};
    *next_offset = get_line((mapped + P6_OFFSET), buffer, 1024);
    return atoi(buffer);
}

int get_height(void *mapped, int offset, int *next_offset)
{
    char buffer[1024] = {};
    *next_offset = get_line((mapped + offset + 1), buffer, 1024);
    return atoi(buffer);
}

int get_line(void *source, char *target, int size)
{
    for (int i = 0; i < size; ++i)
    {
        target[i] = ((char *)source)[i];
        if (target[i] == '\n')
        {
            target[i] = '\0';
            return i;
        }
    }
}

char *parse_args_get_file_name(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Bad argument count");
        exit(EXIT_FAILURE); // todo: exception
    }
    return argv[1];
}

/*
 a b c
 d e f
 g h i
*/
void convalute(unsigned char *image, int *mask, int width, int height, unsigned int *histogram)
{
    unsigned char *copy = malloc(width * height * 3 * sizeof(unsigned char));
    memcpy(copy, image, width * height * 3);
    int i = 0;
    for (int x_axis = 1; x_axis < width - 1; ++x_axis)
    {
        for (int y_axis = 1; y_axis < height - 1; ++y_axis)
        {
            ++i;
            unsigned char a_red, b_red, c_red, d_red, e_red, f_red, g_red, h_red, i_red;
            unsigned char a_green, b_green, c_green, d_green, e_green, f_green, g_green, h_green, i_green;
            unsigned char a_blue, b_blue, c_blue, d_blue, e_blue, f_blue, g_blue, h_blue, i_blue;

            getPixel(copy, width, x_axis-1, y_axis-1, &a_red, &a_green, &a_blue);  
            getPixel(copy, width, x_axis,   y_axis-1, &b_red, &b_green, &b_blue);  
            getPixel(copy, width, x_axis+1, y_axis-1, &c_red, &c_green, &c_blue);  
            
            getPixel(copy, width, x_axis-1, y_axis,   &d_red, &d_green, &d_blue);  
            getPixel(copy, width, x_axis,   y_axis,   &e_red, &e_green, &e_blue);  
            getPixel(copy, width, x_axis+1, y_axis,   &f_red, &f_green, &f_blue);  
            
            getPixel(copy, width, x_axis-1, y_axis+1, &g_red, &g_green, &g_blue);  
            getPixel(copy, width, x_axis,   y_axis+1, &h_red, &h_green, &h_blue);  
            getPixel(copy, width, x_axis+1, y_axis+1, &i_red, &i_green, &i_blue);  

            int new_value_red = 
            a_red * mask[0] + b_red * mask[1] + c_red * mask[2] + 
            d_red * mask[3] + e_red * mask[4] + f_red * mask[5] + 
            g_red * mask[6] + h_red * mask[7] + i_red * mask[8];
        
            int new_value_green = 
            a_green * mask[0] + b_green * mask[1] + c_green * mask[2] + 
            d_green * mask[3] + e_green * mask[4] + f_green * mask[5] + 
            g_green * mask[6] + h_green * mask[7] + i_green * mask[8];
        
            int new_value_blue = 
            a_blue * mask[0] + b_blue * mask[1] + c_blue * mask[2] + 
            d_blue * mask[3] + e_blue * mask[4] + f_blue * mask[5] + 
            g_blue * mask[6] + h_blue * mask[7] + i_blue * mask[8];        
        
            if (new_value_red > 255) new_value_red= 255;
            if (new_value_red < 0) new_value_red= 0;

            if (new_value_green > 255) new_value_green = 255;
            if (new_value_green < 0) new_value_green = 0;

            if (new_value_blue > 255) new_value_blue = 255;
            if (new_value_blue < 0) new_value_blue = 0;

            // Store final values as unsigned char
            unsigned char new_value_red_c = (unsigned char) new_value_red;
            unsigned char new_value_green_c = (unsigned char) new_value_green;
            unsigned char new_value_blue_c = (unsigned char) new_value_blue;

                // Update histogram for Red channel
            if (new_value_red_c <= 50) {
                histogram[0] += 1;
            } else if (new_value_red_c <= 101) {
                histogram[1] += 1;
            } else if (new_value_red_c <= 152) {
                histogram[2] += 1;
            } else if (new_value_red_c <= 203) {
                histogram[3] += 1;
            } else {  // Covers 204-255
                histogram[4] += 1;
            }

            // Update histogram for Green channel
            if (new_value_green_c <= 50) {
                histogram[0] += 1;
            } else if (new_value_green_c <= 101) {
                histogram[1] += 1;
            } else if (new_value_green_c <= 152) {
                histogram[2] += 1;
            } else if (new_value_green_c <= 203) {
                histogram[3] += 1;
            } else {  // Covers 204-255
                histogram[4] += 1;
            }

            // Update histogram for Blue channel
            if (new_value_blue_c <= 50) {
                histogram[0] += 1;
            } else if (new_value_blue_c <= 101) {
                histogram[1] += 1;
            } else if (new_value_blue_c <= 152) {
                histogram[2] += 1;
            } else if (new_value_blue_c <= 203) {
                histogram[3] += 1;
            } else {  // Covers 204-255
                histogram[4] += 1;
            }
            

            setPixel(image, width, x_axis, y_axis, new_value_red_c, new_value_green_c, new_value_blue_c);
        }
    }
    // printf("%i", i);
}

void setPixel(unsigned char *image, int image_width, int x, int y, unsigned char red, unsigned char green, unsigned char blue)
{
    int index = (y * image_width + x) * 3;
    image[index] = red;
    image[index + 1] = green;
    image[index + 2] = blue;
}

void getPixel(unsigned char *image, int image_width, int x, int y, unsigned char *red, unsigned char *green, unsigned char *blue) {
    int index = (y * image_width + x) * 3;
    *red = image[index];
    *green = image[index + 1];
    *blue = image[index + 2];
}