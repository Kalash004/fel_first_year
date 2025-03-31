#include "stdio.h"
#include "stdlib.h"
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>   // open()
#include <sys/mman.h> // mmap(), munmap()
#include <sys/stat.h> // fstat()
#include <unistd.h>   // close()

#define OUTPUT_IMAGE_NAME "./output.ppm" 
#define P6_OFFSET 3

char *parse_args_get_file_name(int argc, char *argv[]);
int get_width(void *mapped, int *next_offset);
int get_height(void *mapped, int offset, int *next_offset);
int get_line(void *source, char *target, int size, int start);


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
    if (!file_name) {
        return EXIT_FAILURE;
    }
    int file_descriptor = open(file_name, O_RDONLY);
    if (file_descriptor == -1) {
        printf("\nException during image file reading\n");
        return EXIT_FAILURE;
    }
    struct stat sb;
    if (fstat(file_descriptor, &sb) == -1) { // Get file size
        printf("\nException during fstat\n");
        return EXIT_FAILURE;
    }
    size_t filesize = sb.st_size;
    void *mapped_mem = mmap(NULL, filesize, PROT_READ, MAP_SHARED, file_descriptor, 0);
    if (mapped_mem == MAP_FAILED) {
        printf("\nException during mmap\n");
        return EXIT_FAILURE;
    }


    int next_skip = 0;
    int width = get_width(mapped_mem, &next_skip);
    int height = get_height(mapped_mem, P6_OFFSET + next_skip, &next_skip);

    char *image = mapped_mem

}

int get_width(void *mapped, int *next_offset) {
    char buffer[1024] = {};
    *next_offset = get_line((mapped + P6_OFFSET), buffer, 1024);
    return atoi(buffer);
}

int get_height(void *mapped, int offset, int *next_offset) {
    char buffer[1024] = {};
    *next_offset = get_line((mapped + offset + 1), buffer, 1024);
    return atoi(buffer);
}

int get_line(void *source, char *target, int size, int start) {
    static int _start = 0;
    if (start != 0 ){
        _start = start;
    }
    for (int i = start; i < size; ++i) {
        target[i] = ((char *)source)[i];
        if (target[i] == '\n') {
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
