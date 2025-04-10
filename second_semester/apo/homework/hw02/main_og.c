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

void convolute(unsigned char *image, int width, int height, unsigned int *histogram);
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

    int width = 0;
    int height = 0;
    int i = 3;

    char temp[100000] = {};
    int inx = 0;
    for (; ((char *)mapped_mem)[i] != '\n'; ++i)
    {
        temp[inx] = ((char *)mapped_mem)[i];
        ++inx;
    }
    ++i;
    temp[++inx] = '\0';
    width = atoi(temp);

    char temp2[100000] = {};
    inx = 0;
    for (; ((char *)mapped_mem)[i] != '\n'; ++i)
    {
        temp2[inx] = ((char *)mapped_mem)[i];
        ++inx;
    }
    ++i;
    temp2[++inx] = '\0';
    height = atoi(temp2);
    i += 4;
    size_t n = width * height * 3 * sizeof(unsigned char);
    unsigned char *read_img = malloc(n);
    memcpy(read_img, (unsigned char *)(mapped_mem + i), n);

    unsigned int histogram[5] = {0};
    convolute(read_img, width, height, histogram);

    for (int x = 0; x < width; ++x)
    {
        unsigned int r, g, b;
        int idx_center = x * 3;
        r = read_img[idx_center];
        g = read_img[idx_center + 1];
        b = read_img[idx_center + 2];
        int grey = round(0.2126 * r + 0.7152 * g + 0.0722 * b);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;

        idx_center = ((height - 1) * width + x) * 3;
        r = read_img[idx_center];
        g = read_img[idx_center + 1];
        b = read_img[idx_center + 2];
        grey = round(0.2126 * r + 0.7152 * g + 0.0722 * b);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
    }

    for (int y = 1; y < height - 1; ++y)
    {
        unsigned int r, g, b;
        int idx_center = (y * width) * 3;
        r = read_img[idx_center];
        g = read_img[idx_center + 1];
        b = read_img[idx_center + 2];
        int grey = round(0.2126 * r + 0.7152 * g + 0.0722 * b);
        int bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;

        idx_center = (y * width + (width - 1)) * 3;
        r = read_img[idx_center];
        g = read_img[idx_center + 1];
        b = read_img[idx_center + 2];
        grey = round(0.2126 * r + 0.7152 * g + 0.0722 * b);
        bin = grey / 51;
        if (bin > 4)
            bin = 4;
        histogram[bin]++;
    }

    FILE *histo_txt = fopen(OUTPUT_HISTOGRAM_NAME, "w");
    fprintf(histo_txt, "%u %u %u %u %u", histogram[0], histogram[1], histogram[2], histogram[3], histogram[4]);
    fclose(histo_txt);

    FILE *save_to = fopen(OUTPUT_IMAGE_NAME, "wb");
    fprintf(save_to, "P6\n%d\n%d\n255\n", width, height);
    fwrite(read_img, sizeof(unsigned char), (width * height * 3 * sizeof(unsigned char)), save_to);
    free(read_img);
    fclose(save_to);
    close(file_descriptor);
}

void convolute(unsigned char *image, int width, int height, unsigned int *histogram)
{
    unsigned char *copy = malloc(width * height * 3 * sizeof(unsigned char));
    memcpy(copy, image, width * height * 3 * sizeof(unsigned char));

    for (int y = 1; y < height - 1; ++y)
    {
        for (int x = 1; x < width - 1; ++x)
        {

            int idx_center = (y * width + x) * 3;

            int idx_b = ((y - 1) * width + x) * 3; // Top
            int idx_d = (y * width + (x - 1)) * 3; // Left
            int idx_f = (y * width + (x + 1)) * 3; // Right
            int idx_h = ((y + 1) * width + x) * 3; // Bottom

            int new_r = 5 * copy[idx_center] - copy[idx_b] - copy[idx_d] - copy[idx_f] - copy[idx_h];
            int new_g = 5 * copy[idx_center + 1] - copy[idx_b + 1] - copy[idx_d + 1] - copy[idx_f + 1] - copy[idx_h + 1];
            int new_b = 5 * copy[idx_center + 2] - copy[idx_b + 2] - copy[idx_d + 2] - copy[idx_f + 2] - copy[idx_h + 2];

            new_r = (new_r < 0) ? 0 : (new_r > 255) ? 255
                                                    : new_r;
            new_g = (new_g < 0) ? 0 : (new_g > 255) ? 255
                                                    : new_g;
            new_b = (new_b < 0) ? 0 : (new_b > 255) ? 255
                                                    : new_b;

            image[idx_center] = (unsigned char)new_r;
            image[idx_center + 1] = (unsigned char)new_g;
            image[idx_center + 2] = (unsigned char)new_b;

            int grey = round(0.2126 * new_r + 0.7152 * new_g + 0.0722 * new_b);

            int bin = grey / 51;
            if (bin > 4)
                bin = 4;
            histogram[bin]++;
        }
    }

    free(copy);
}
