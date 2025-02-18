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


int detect_lang(Options *opt, Dictionary *dict) {
    FILE *to_detect_lang_file = fopen(opt->file_to_be_translated);
    char word[100];
    read_word(to_detect_lang_file, word);   
}


void find_translation(Options config,MyTranslation * translations,int translations_size ,char *word, int *result) {
    for (int i = 0; i < translations_size; ++i) {
        MyTranslation t;
        t = translations[i];
        if (strcmp(word, t.word_in_foreign) == 0) { // possible optimization
            *result = 2;
        }
          if (config.lang_column_used_in_file == 1 && strcmp(word, t.word_in_english) == 0) { // possible optimization
            strcpy(dest, t.word_in_foreign);
            break;
        }
    }
}