#include "stdio.h"
#include "stdlib.h"
#include <sys/mman.h>

char *parse_args_get_file_name(int argc, char *argv[]);

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
        return -1;
    }
    FILE *target = fopen(file_name, "r");
    fclose(target);
    
}

char *parse_args_get_file_name(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Bad argument count");
        exit(-1); // todo: exception
    }
    return argv[1];
}
