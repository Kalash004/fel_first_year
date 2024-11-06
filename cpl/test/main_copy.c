#include <stdio.h>
#include <string.h>


typedef struct
{
    char *dict_file_path;
    char *file_to_be_translated;
    int lang_column_used_in_file;
} Options;

typedef struct
{
    char word_in_english[100];
    char word_in_foreign[100]; 
} MyTranslation;

typedef struct
{
    size_t dict_size;
    MyTranslation *translations;
} Dictionary;

int main(int argc, char *argv[]) {
    Options config;
    int arg_read_result = argument_handling(&config,argc,argv);
    if (arg_read_result != 1) {
        fprintf(stderr, "Arg read error");
        return 2;
    }

    if (config.lang_column_used_in_file == 0) {
        fprintf(stderr, "");
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
    // printf("%s - %s\n", arr[0].word_in_english, arr[0].word_in_foreign);
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
    if (*argv[3] == 49)
        num = 0;
    __dest->lang_column_used_in_file = num; // check for int
    return 1;
}

int get_file_line_count(FILE *f)
{
    fseek(f, 0, SEEK_SET);
    char buffer[500];
    int count = 0;
    while (!feof(f))
    {
        fgets(buffer, 500, f);
        ++count;
    }
    fseek(f, 0, SEEK_SET);
    return count;
}

void read_all_translation_lines(FILE *f, MyTranslation *dest, int size) {
    int i = 0;
    while (!feof(f)) {
        if (i > size) 
            break;
        char word_in_eng[100];
        char word_in_other[100];
        read_translation_line(f, word_in_eng, word_in_other);
        MyTranslation t;
        strcpy(t.word_in_english,word_in_eng);
        strcpy(t.word_in_foreign,word_in_other);
        dest[i] = t;
        ++i;
    }
}

void read_translation_line(FILE *f, char *word_in_eng, char *word_in_other) {
    fscanf(f,"%s %s\n", word_in_eng, word_in_other);       
}

int translate_file(Options config, Dictionary *dict) {
    FILE *f = fopen(config.file_to_be_translated, "r");
    int i = 0;
    while (!feof(f)) {
        char word_in_to_be_translated[100]; 
        int result = read_word(f, word_in_to_be_translated);
        if (result == -1)
            break;
        printf(" ");
        char translation[100];
        find_translation(config,dict->translations, dict->dict_size, word_in_to_be_translated, translation);
        printf("%s", translation);
        fflush(stdout);
        ++i;
    }
    printf("\n");
    fclose(f);
}

int read_word(FILE *f, char *word) {
    int i = 0;
    while (!feof(f)) {
        char c = fgetc(f);
        if (c == ' ' || c == '\n')
            break;

        if (c == EOF) 
            return -1;
        
        word[i] = c;
        ++i;
    }
    word[i] = '\0';
    return i;
}

void find_translation(Options config,MyTranslation * translations,int translations_size ,char *word, char *dest) {
    for (int i = 0; i < translations_size; ++i) {
        MyTranslation t;
        t = translations[i];
        if (config.lang_column_used_in_file == 2 && strcmp(word, t.word_in_foreign) == 0) { // possible optimization
            strcpy(dest, t.word_in_english);
            break;
        }
          if (config.lang_column_used_in_file == 1 && strcmp(word, t.word_in_english) == 0) { // possible optimization
            strcpy(dest, t.word_in_foreign);
            break;
        }
    }
}

