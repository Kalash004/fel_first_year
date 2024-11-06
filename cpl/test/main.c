#include "translation_operations.h"
int argument_handling(Options *__dest,int argc, char *argv[]);

int main(int argc, char *argv[]) {
    Options config;
    int arg_read_result = argument_handling(&config,argc,argv);
    if (arg_read_result != 1) {
        fprintf(stderr, "Arg read error");
        return 2;
    }

    if (config.lang_column_used_in_file == 0) {
        fprintf(stderr, "Lang detection not implemented\n");
        return 3;
    } 

    FILE *dict_file = fopen(config.dict_file_path, "r");

    if (dict_file == NULL){
        fprintf(stderr, "dict file == null");
        return 2;
    }    
    int line_count = get_file_line_count(dict_file);
    
    MyTranslation arr[line_count];
    read_all_translation_lines(dict_file, arr, line_count);
    fclose(dict_file);
    Dictionary dict = {
        .dict_size = line_count,
        .translations = arr
    };
    // - Dictionary reading end

    translate_file(config, &dict);
    return 0;
}

int argument_handling(Options *__dest,int argc, char *argv[]) { // done
    if (argc != 4) 
        return -1;

    __dest->dict_file_path = argv[1];
    __dest->file_to_be_translated = argv[2];
    int num;
    if (*argv[3] == 49)
        num = 1;
    if (*argv[3] == 50)
        num = 2;
    if (*argv[3] == 48)
        num = 0;

    __dest->lang_column_used_in_file = num; // check for int
    return 1;
}
