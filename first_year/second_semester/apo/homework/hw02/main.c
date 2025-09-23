#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>    // open()
#include <sys/mman.h> // mmap(), munmap()
#include <sys/stat.h> // fstat()
#include <unistd.h>   // close()
#include <string.h>   // For memcpy()
#include <math.h>

#define OUTPUT_IMAGE_NAME "./output.ppm"
#define OUTPUT_HISTOGRAM_NAME "./output.txt"
#define P6_OFFSET 3
#define MAX_COLOR_OFFSET 4

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} pixel_t;

static inline void convolute(unsigned char *old, pixel_t *save_to, int width, int height, unsigned int *histogram);

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

    // args
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

    int width = 0;
    int height = 0;
    int offset = 3;

    char temp[100000] = {};
    int inx = 0;
    for (; ((char *)mapped_mem)[offset] != '\n'; ++offset)
    {
        temp[inx] = ((char *)mapped_mem)[offset];
        ++inx;
    }
    ++offset;
    temp[++inx] = '\0';
    width = atoi(temp);

    char temp2[100000] = {};
    inx = 0;
    for (; ((char *)mapped_mem)[offset] != '\n'; ++offset)
    {
        temp2[inx] = ((char *)mapped_mem)[offset];
        ++inx;
    }
    ++offset;
    temp2[++inx] = '\0';
    height = atoi(temp2);
    offset += 4;

    size_t n = width * height;

    unsigned int histogram[5] = {0};
    unsigned char *read_img = (unsigned char *)(mapped_mem + offset);

    pixel_t *new_img = calloc(n, sizeof(pixel_t));

    for (int x = 0; x < width; ++x)
    {
        int idx_center = x * 3;
        int grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];


        idx_center = ((height - 1) * width + x) * 3;
        grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];
    }

    for (int y = 1; y < height - 1; ++y)
    {
        int idx_center = (y * width) * 3;
        int grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];

        idx_center = (y * width + (width - 1)) * 3;
        grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];
    }
    pixel_t *new_img = calloc(n, sizeof(pixel_t));

    for (int x = 0; x < width; ++x)
    {
        int idx_center = x * 3;
        int grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];


        idx_center = ((height - 1) * width + x) * 3;
        grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];
    }

    for (int y = 1; y < height - 1; ++y)
    {
        int idx_center = (y * width) * 3;
        int grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];

        idx_center = (y * width + (width - 1)) * 3;
        grey = round(0.2126 * read_img[idx_center] + 0.7152 * read_img[idx_center + 1] + 0.0722 * read_img[idx_center + 2]);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
        new_img[idx_center/3].r = read_img[idx_center];
        new_img[idx_center/3].g = read_img[idx_center + 1];
        new_img[idx_center/3].b = read_img[idx_center + 2];
    }


    convolute(read_img, new_img, width, height, histogram);

    FILE *histo_txt = fopen(OUTPUT_HISTOGRAM_NAME, "w");
    fprintf(histo_txt, "%u %u %u %u %u", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4]);
    fclose(histo_txt);

    FILE *save_to = fopen(OUTPUT_IMAGE_NAME, "wb");
    fprintf(save_to, "P6\n%d\n%d\n255\n", width, height);
    fwrite(new_img, sizeof(pixel_t), n, save_to);

    free(new_img);
    fclose(save_to);
    close(file_descriptor);
}

static inline void convolute(unsigned char *read_img, pixel_t *save_to, int width, int height, unsigned int *histogram)
{
    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {
            int idx_center = (y * width + x);

            unsigned char *current_pixel = read_img + (y * width + x) * 3;
            unsigned char *top_pixel = read_img + ((y - 1) * width + x) * 3; // Top
            unsigned char *left_pixel = read_img + (y * width + (x - 1)) * 3; // Left
            unsigned char *right_pixel = read_img + (y * width + (x + 1)) * 3; // Right
            unsigned char *bottom_pixel = read_img + ((y + 1) * width + x) * 3; // Bottom

            int new_r = 5 * current_pixel[0] - top_pixel[0] - left_pixel[0] - right_pixel[0] - bottom_pixel[0];
            int new_g = 5 * current_pixel[1] - top_pixel[1] - left_pixel[1] - right_pixel[1] - bottom_pixel[1];
            int new_b = 5 * current_pixel[2] - top_pixel[2] - left_pixel[2] - right_pixel[2] - bottom_pixel[2];

            new_r = (new_r < 0) ? 0 : (new_r > 255) ? 255
                                                    : new_r;
            new_g = (new_g < 0) ? 0 : (new_g > 255) ? 255
                                                    : new_g;
            new_b = (new_b < 0) ? 0 : (new_b > 255) ? 255
                                                    : new_b;

            save_to[idx_center].r = (unsigned char)new_r;
            save_to[idx_center].g = (unsigned char)new_g;
            save_to[idx_center].b = (unsigned char)new_b;

            int grey = round(0.2126 * new_r + 0.7152 * new_g + 0.0722 * new_b);

            int bin = grey / 51;
            if (bin > 4)
                bin = 4;
            histogram[bin]++;
        }
    }
}
