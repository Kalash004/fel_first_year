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
#include <math.h>
#include <immintrin.h>  // AVX2 Intrinsics

#define OUTPUT_IMAGE_NAME "./testoutput.ppm"
#define P6_OFFSET 3
#define MAX_COLOR_OFFSET 4

char *parse_args_get_file_name(int argc, char *argv[]);
static inline void setPixel(unsigned char *image, int image_width, int x, int y, unsigned char red, unsigned char green, unsigned char blue);
static inline void getPixel(unsigned char *image, int image_width, int x, int y, unsigned char *red, unsigned char *green, unsigned char *blue);
void convolute(unsigned char *image, int *mask, int width, int height, unsigned int *histogram);

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

    if (argc != 2)
    {
        printf("Bad argument count");
        exit(EXIT_FAILURE); // todo: exception
    }
    char *file_name = argv[1];
    if (!file_name)
    {
        printf("File doesnt exist\n");
        return EXIT_FAILURE;
    }


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
    void *mapped_mem = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_descriptor, 0);
    if (mapped_mem == MAP_FAILED)
    {
        printf("\nException during mmap\n");
        return EXIT_FAILURE;
    }


    
    close(file_descriptor);
    
    int width = 0;
    int height = 0;
    
    int i = 3; 
    
    char temp[100000];
    int inx = 0;
    for (i; ((char *)mapped_mem)[i] != '\n'; ++i) {
        temp[inx] = ((char *)mapped_mem)[i];
        ++inx;
    }
    ++i;
    temp[++inx] = '\0';
    width = atoi(temp);
    
    
    char temp2[100000];
    inx = 0;
    for (i; ((char *)mapped_mem)[i] != '\n'; ++i) {
        temp2[inx] = ((char *)mapped_mem)[i];
        ++inx;
    }
    ++i;
    temp2[++inx] = '\0';
    height = atoi(temp2);
    i+=4;
    size_t n = width * height * 3 * sizeof(unsigned char);
    unsigned char *image = malloc(n);
    memcpy(image, (unsigned char *)(mapped_mem + i), n);
    
    
    unsigned int histogram[5];
    int mask[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
    convolute(image, mask, width, height, histogram);
    
    for (int x = 0; x < width; ++x) {
        unsigned char r, g, b;
        getPixel(image, width, x, 0, &r, &g, &b);
        int grey = round(0.2126 * r +  0.7152 * g + 0.0722 * b);
   
        if (grey <= 50) {
            histogram[0] += 1;
        } else if (grey <= 101) {
            histogram[1] += 1;
        } else if (grey <= 152) {
            histogram[2] += 1;
        } else if (grey <= 203) {
            histogram[3] += 1;
        } else if (grey <= 255) { 
            histogram[4] += 1;
        }

        getPixel(image, width, x, height, &r, &g, &b);
        grey = round(0.2126 * r +  0.7152 * g + 0.0722 * b);

        if (grey <= 50) {
            histogram[0] += 1;
        } else if (grey <= 101) {
            histogram[1] += 1;
        } else if (grey <= 152) {
            histogram[2] += 1;
        } else if (grey <= 203) {
            histogram[3] += 1;
        } else if (grey <= 255) { 
            histogram[4] += 1;
        }     
        
    }

    for (int y = 1; y < height - 1; ++y ) {
        unsigned char r, g, b;

        getPixel(image, width, 0, y, &r, &g, &b);
        int grey = round(0.2126 * r +  0.7152 * g + 0.0722 * b);

        if (grey <= 50) {
            histogram[0] += 1;
        } else if (grey <= 101) {
            histogram[1] += 1;
        } else if (grey <= 152) {
            histogram[2] += 1;
        } else if (grey <= 203) {
            histogram[3] += 1;
        } else if (grey <= 255) { 
            histogram[4] += 1;
        }
    
        getPixel(image, width, width, y, &r, &g, &b);
        grey = round(0.2126 * r +  0.7152 * g + 0.0722 * b);

        if (grey <= 50) {
            histogram[0] += 1;
        } else if (grey <= 101) {
            histogram[1] += 1;
        } else if (grey <= 152) {
            histogram[2] += 1;
        } else if (grey <= 203) {
            histogram[3] += 1;
        } else if (grey <= 255) { 
            histogram[4] += 1;
        }
    
    }


    printf("histogram : %u %u %u %u %u\n", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4]);


    FILE *save_to = fopen(OUTPUT_IMAGE_NAME, "wb");
    fprintf(save_to, "P6\n%d %d\n255\n", width, height);
    fwrite(image, sizeof(unsigned char), (width * height * 3), save_to);    
    free(image);

}

/*
 a b c
 d e f
 g h i
*/

/*
// void convolve_SIMD(unsigned char *image, int *mask, int width, int height, unsigned int *histogram) {
    //     unsigned char *copy = (unsigned char*)malloc(width * height * 3);  
    //     memcpy(copy, image, width * height * 3);
    
    //     for (int y = 1; y < height - 1; y++) {
        //         for (int x = 1; x < width - 1; x += 8) {  // Process 8 pixels at a time
        
        //             // Load 9 neighboring pixels into SIMD registers
        //             __m256i a = _mm256_loadu_si256((__m256i*)&copy[((y - 1) * width + (x - 1)) * 3]);
        //             __m256i b = _mm256_loadu_si256((__m256i*)&copy[((y - 1) * width + (x)) * 3]);
        //             __m256i c = _mm256_loadu_si256((__m256i*)&copy[((y - 1) * width + (x + 1)) * 3]);
        //             __m256i d = _mm256_loadu_si256((__m256i*)&copy[((y) * width + (x - 1)) * 3]);
        //             __m256i e = _mm256_loadu_si256((__m256i*)&copy[((y) * width + (x)) * 3]);
        //             __m256i f = _mm256_loadu_si256((__m256i*)&copy[((y) * width + (x + 1)) * 3]);
        //             __m256i g = _mm256_loadu_si256((__m256i*)&copy[((y + 1) * width + (x - 1)) * 3]);
        //             __m256i h = _mm256_loadu_si256((__m256i*)&copy[((y + 1) * width + (x)) * 3]);
        //             __m256i i = _mm256_loadu_si256((__m256i*)&copy[((y + 1) * width + (x + 1)) * 3]);
        
        //             // Convert to 16-bit integers (avoids overflow)
        //             __m256i a16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(a));
        //             __m256i b16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(b));
        //             __m256i c16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(c));
        //             __m256i d16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(d));
        //             __m256i e16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(e));
        //             __m256i f16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(f));
        //             __m256i g16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(g));
        //             __m256i h16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(h));
        //             __m256i i16 = _mm256_cvtepu8_epi16(_mm256_castsi256_si128(i));
        
        //             // Multiply with mask
        //             __m256i result = _mm256_add_epi16(
            //                 _mm256_mullo_epi16(a16, _mm256_set1_epi16(mask[0])),
            //                 _mm256_mullo_epi16(b16, _mm256_set1_epi16(mask[1]))
            //             );
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(c16, _mm256_set1_epi16(mask[2])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(d16, _mm256_set1_epi16(mask[3])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(e16, _mm256_set1_epi16(mask[4])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(f16, _mm256_set1_epi16(mask[5])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(g16, _mm256_set1_epi16(mask[6])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(h16, _mm256_set1_epi16(mask[7])));
            //             result = _mm256_add_epi16(result, _mm256_mullo_epi16(i16, _mm256_set1_epi16(mask[8])));
            
            //             // Normalize & Clamp (0-255)
            //             result = _mm256_max_epi16(_mm256_min_epi16(result, _mm256_set1_epi16(255)), _mm256_setzero_si256());
            
            //             // Store result
            //             _mm256_storeu_si256((__m256i*)&image[(y * width + x) * 3], result);
            //         }
            //     }
            //     free(copy);
            // }
            */


void convolute(unsigned char *image, int *mask, int width, int height, unsigned int *histogram)
{
    unsigned char *copy = malloc(width * height * 3 * sizeof(unsigned char));
    memcpy(copy, image, width * height * 3);
    int i = 0;
    for (int x_axis = 1; x_axis < width - 1; ++x_axis)
    {
        for (int y_axis = 1; y_axis < height - 1; ++y_axis)
        {
            unsigned char b_red, d_red, e_red, f_red, h_red;
            unsigned char b_green, d_green, e_green, f_green, h_green;
            unsigned char b_blue, d_blue, e_blue, f_blue, h_blue;

            getPixel(image, width, x_axis,   y_axis-1, &b_red, &b_green, &b_blue);  
            
            getPixel(image, width, x_axis-1, y_axis,   &d_red, &d_green, &d_blue);  
            getPixel(image, width, x_axis,   y_axis,   &e_red, &e_green, &e_blue);  
            getPixel(image, width, x_axis+1, y_axis,   &f_red, &f_green, &f_blue);  
            
            getPixel(image, width, x_axis,   y_axis+1, &h_red, &h_green, &h_blue);  

            int new_value_red = b_red * mask[1] + d_red * mask[3] + e_red * mask[4] + f_red * mask[5] + h_red * mask[7];
            int new_value_blue = b_blue * mask[1] + d_blue * mask[3] + e_blue * mask[4] + f_blue * mask[5] + h_blue * mask[7];
            int new_value_green = b_green * mask[1] + d_green * mask[3] + e_green * mask[4] + f_green * mask[5] + h_green * mask[7];
        
            if (new_value_red > 255) new_value_red= 255;
            if (new_value_red < 0) new_value_red= 0;

            if (new_value_green > 255) new_value_green = 255;
            if (new_value_green < 0) new_value_green = 0;

            if (new_value_blue > 255) new_value_blue = 255;
            if (new_value_blue < 0) new_value_blue = 0;

            unsigned char new_value_red_c = (unsigned char) new_value_red;
            unsigned char new_value_green_c = (unsigned char) new_value_green;
            unsigned char new_value_blue_c = (unsigned char) new_value_blue;
            
            int y = round(0.2126 * new_value_red_c +  0.7152 * new_value_green_c + 0.0722 * new_value_blue_c);

            if (y <= 50) {
                histogram[0] += 1;
            } else if (y <= 101) {
                histogram[1] += 1;
            } else if (y <= 152) {
                histogram[2] += 1;
            } else if (y <= 203) {
                histogram[3] += 1;
            } else if (y <= 255) { 
                histogram[4] += 1;
            }

            setPixel(copy, width, x_axis, y_axis, new_value_red_c, new_value_green_c, new_value_blue_c);
        }
    }
    free(copy);
}

static inline void setPixel(unsigned char *image, int width, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    unsigned char *ptr = image + ((y * width + x) * 3);
    ptr[0] = r;
    ptr[1] = g;
    ptr[2] = b;
}

static inline void getPixel(unsigned char *image, int width, int x, int y, unsigned char *r, unsigned char *g, unsigned char *b) {
    unsigned char *ptr = image + ((y * width + x) * 3);
    *r = ptr[0];
    *g = ptr[1];
    *b = ptr[2];
}
