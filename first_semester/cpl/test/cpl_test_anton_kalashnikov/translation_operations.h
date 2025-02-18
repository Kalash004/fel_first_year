#include <stdio.h>
#include <string.h>

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

typedef struct
{
    char *dict_file_path;
    char *file_to_be_translated;
    int lang_column_used_in_file;
} Options;

/// @brief Get count of lines in file. After done returns cursor to start of file
/// @param f File to count
/// @return int - number of lines
int get_file_line_count(FILE *f);
/// @brief Reads dictionary lines, saves MyTraslation(word in english, word in foreign language) structs to destination for each word 
/// @param f Dictionary file
/// @param dest Pointer to MyTranslation array 
/// @param size Line count
void read_all_translation_lines(FILE *f, MyTranslation *dest, int size);
/// @brief Reads line and saves words to the pointer
/// @param f Dictionary file
/// @param word_in_eng Where to save word in col 1
/// @param word_in_other Where to save word in col 2
void read_translation_line(FILE *f, char *word_in_eng, char *word_in_other);
/// @brief Reads one word form file
/// @param f File to read from
/// @param word Destination to save to 
/// @return count of chars or -1 on eof
int read_word(FILE *f, char *word);
/// @brief Read word, find translation, print translation. For each word
/// @param config Config file
/// @param dict Dictionary object
/// @return int word count
int translate_file(Options config, Dictionary *dict);
/// @brief Find translation of word from file
/// @param config Config file
/// @param translations Array of translation objects
/// @param translations_size Size of array ^
/// @param word To find in the translations
/// @param dest Return translation to
void find_translation(Options config,MyTranslation * translations,int translations_size ,char *word, char *dest);
#pragma once

