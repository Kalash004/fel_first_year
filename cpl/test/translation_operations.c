#include "translation_operations.h"

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
    return i;
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
