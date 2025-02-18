#include "./my_str.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    bool is_end_cell;
    bool is_prohibition_group;
    bool is_regex_group;
    char *regex;
} My_Regex;

size_t _get_str_size(const char *_source);
void fill_target_with_token(char *temp, size_t token_start, size_t token_end, char *str);
size_t find_next_delim_end(char *str, const char *delim, size_t *str_loc, size_t delim_size, bool *done_flag, bool *delim_found_flag);
size_t get_delim_size(const char *delim);
int is_char_in_str(char c, const char *string);
size_t get_first_non_delim_char_location(char *str, const char *delim, size_t delim_size);
/// @brief Returns pattern size
size_t get_pattern_real_size(const char *pattern);
/// @brief Returns group size
size_t get_group_size(size_t pattern_index, const char *pattern);
/// @brief Handles saving of group regex to struct
My_Regex *handle_group(size_t pattern_index, const char *pattern);
/// @brief Saves pattern into regex struct
/// @param pattern String pattern
/// @return Pointer to regex array
My_Regex **load_patter_into_struct(const char *pattern);
/// @brief Checks if character is inside a group of regex
/// @param reg Regex group
/// @param c Character to check for
/// @return True if inside
bool is_c_in_reg_group(My_Regex *reg, const char c);
/// @brief Checks if character is contained in regex
/// @param regex Regex to check
/// @param c Character to check
/// @param last_reg_id Last id of used regex
/// @return Index of regex that contains the char
long int is_c_in_regex(My_Regex **regex, const char c, size_t last_reg_id);
/// @brief Traverses the string given and looks for pattern in it.
/// @param regex Pattern saved to My_Regex array
/// @param str String to traverse
/// @param regex_size Size of the regex
/// @return Index of the word found by regex
long int regex_looker(My_Regex **regex, const char *str, size_t regex_size);


void my_strcat(char **_dest, const char **_source)
{
    // find size of dest
    size_t _dest_size = _get_str_size(*_dest);
    // find sie of source
    size_t _source_size = _get_str_size(*_source);
    // realloc depending on size
    if (_dest_size - 1 + _source_size == _dest_size) // works only if source is 1 char and it is /n so no point in changing the str
        return;
    char *_temp = (char *)realloc(*_dest, (_dest_size + _source_size - 1) * sizeof(char));
    if (_temp == NULL)
    {
        // TODO: define behaviour when realloc fails
        return;
    }
    size_t used = _dest_size - 1;
    for (int i = 0; i < _source_size; ++i)
    {
        _temp[used] = (*_source)[i];
        ++used;
    }
    *_dest = _temp;
}

size_t _get_str_size(const char *_source)
{
    size_t str_size = 1;
    do
    {
        if (_source[str_size - 1] == '\0')
            break;
        ++str_size;
    } while (1);
    return str_size;
}

unsigned long my_strspn(const char *dest, const char *src)
{
    char c;
    unsigned long i = 0;
    do
    {
        c = dest[i];
        if (is_char_in_str(c, src))
        {
            ++i;
            continue;
        }
        break;
    } while (c != '\0');
    return i;
}

int is_char_in_str(char c, const char *string)
{
    char compare_to;
    size_t i = 0;
    do
    {
        compare_to = string[i];
        if (compare_to == (int)c)
            return 1;
        ++i;
    } while (compare_to != '\0');
    return 0;
}

char *my_strtok(char *str, const char *delim)
{
    static size_t location_in_str;
    static char *save;
    size_t token_start = location_in_str;
    static bool is_done = false;
    static size_t delim_size;

    if (is_done)
    {
        return NULL;
    }

    if (str != NULL)
    {
        delim_size = get_delim_size(delim);
        location_in_str = get_first_non_delim_char_location(str, delim, delim_size);
        save = str;
        token_start = location_in_str;
    }
    bool is_delim_found = false;
    size_t token_end = find_next_delim_end(save, delim, &location_in_str, delim_size, &is_done, &is_delim_found);
    if (is_delim_found)
    {
        token_end -= delim_size;
    }
    const size_t token_size = token_end - token_start;
    char *temp = (char *)malloc((token_size + 1) * sizeof(char));
    fill_target_with_token(temp, token_start, token_end, save);
    return temp;
}

void fill_target_with_token(char *temp, size_t token_start, size_t token_end, char *str)
{
    if (token_start >= token_end)
    {
        return;
    }
    size_t token_id = 0;
    for (size_t i = token_start; i < token_end; ++i)
    {
        temp[token_id] = str[i];
        ++token_id;
    }
    temp[token_id] = '\0';
}

size_t find_next_delim_end(char *str, const char *delim, size_t *str_loc, size_t delim_size, bool *done_flag, bool *delim_found_flag)
{
    char c;
    int delim_index = 0;
    do
    {
        c = str[*str_loc];
        if (c == delim[delim_index])
        {
            if (delim_index == delim_size - 1) // end of delimiter
            {
                *delim_found_flag = true;
                ++*str_loc;
                *done_flag = 0;
                return *str_loc;
            }
            ++delim_index;
        }
        ++*str_loc;
    } while (c != '\0');
    *done_flag = true;
    return *str_loc;
}

size_t get_delim_size(const char *delim)
{
    char c;
    size_t size = 0;
    do
    {
        c = delim[size];
        ++size;
    } while (c != '\0');
    return size - 1;
}

size_t get_first_non_delim_char_location(char *str, const char *delim, size_t delim_size)
{
    size_t location = 0;
    char c;
    do
    {
        c = str[location];
        if (c == delim[0])
        {
            for (size_t i = 1; i < delim_size; ++i)
            {
                if (str[location + i] != delim[i])
                {
                    return location;
                }
            }
            location += delim_size;
        }
        else
        {
            return location;
        }
    } while (c != '\0');
    return location;
}

unsigned long my_strstr_match(const char *str, const char *substr, char **begin)
{
    My_Regex **regex = load_patter_into_struct(substr);
    size_t reg_size = get_pattern_real_size(substr);
    long int start_index = regex_looker(regex, str, reg_size);
    if (start_index == -1)
    {
        *begin = NULL;
        return 0;
    }
    char *temp = malloc(sizeof(char) * (reg_size + 1));
    size_t used = 0;
    for (size_t i = start_index; i < start_index + reg_size; ++i, ++used)
    {
        temp[used] = str[i];
    }
    temp[++used] = '\0';
    *begin = temp;
    return reg_size;
}

long int regex_looker(My_Regex **regex, const char *str, size_t regex_size)
{
    size_t latest_found_regex_id = 0;
    char c;
    size_t i = 0;
    long int latest_regex_start_id = -1;
    do
    {

        c = str[i];
        if (regex_size - 1 == latest_found_regex_id && latest_regex_start_id > -1)
        {
            return latest_regex_start_id;
        }
        long int c_in_reg = is_c_in_regex(regex, c, latest_found_regex_id);
        if (c_in_reg == 0)
        {
            latest_regex_start_id = i;
            ++i;
            continue;
        }
        if (c_in_reg - 1 == latest_found_regex_id)
        {
            latest_found_regex_id = c_in_reg;
            ++i;
            continue;
        }
        if (c_in_reg - 1 != latest_found_regex_id)
        {
            latest_regex_start_id = -1;
            latest_found_regex_id = 0;
        }
        ++i;
    } while (c != '\0');
    return -1;
}

long int is_c_in_regex(My_Regex **regex, const char c, size_t last_reg_id)
{
    size_t i = last_reg_id;
    if (last_reg_id > 0)
    {
        ++i;
    }
    My_Regex *reg;
    do
    {
        reg = regex[i];
        if (reg->is_end_cell)
            return -1;
        if (reg->is_regex_group)
        {
            if (reg->is_prohibition_group)
            {
                if (!is_c_in_reg_group(reg, c))
                    return i;
            }
            if (is_c_in_reg_group(reg, c))
                return i;
            ++i;
            continue;
        }
        if (c == reg->regex[0])
        {
            return i;
        }
        ++i;
    } while (!reg->is_end_cell);
    return -1;
}

bool is_c_in_reg_group(My_Regex *reg, const char c)
{
    char reg_char;
    size_t i = 0;
    do
    {
        reg_char = reg->regex[i];
        if (reg_char == c)
            return true;

        ++i;
    } while (reg_char != '\0');
    return false;
}

My_Regex **load_patter_into_struct(const char *pattern)
{
    size_t pattern_real_size = get_pattern_real_size(pattern);
    My_Regex **regex_array = malloc(sizeof(My_Regex *) * pattern_real_size + 1); // to account for arr_end

    size_t pattern_index = 0;
    size_t loop = 0;
    char c;
    do
    {
        My_Regex *reg;
        c = pattern[pattern_index];
        if (c == '\0')
            break;

        if (c == '[')
        {
            size_t group_length = get_group_size(pattern_index, pattern);
            reg = handle_group(pattern_index, pattern);
            regex_array[loop] = reg;
            ++loop;
            if (reg == NULL)
                return NULL; // TODO: Handle null;
            do
            {
                c = pattern[pattern_index];
                ++pattern_index;
            } while (c != ']');
            continue;
        }
        else
        {
            char *pChar = malloc(sizeof(char) * 2);
            if (pChar == NULL)
                return NULL; // TODO: Handle null
            pChar[0] = c;
            pChar[1] = '\0';
            reg = malloc(sizeof(My_Regex) * 1);
            if (reg == NULL)
                return NULL; // TODO: Handle null
            reg->is_prohibition_group = false;
            reg->is_regex_group = false;
            reg->regex = pChar;
        }
        regex_array[loop] = reg;
        ++pattern_index;
        ++loop;
    } while (c != '\0');
    My_Regex *finish = malloc(sizeof(My_Regex) * 1);
    finish->is_end_cell = true;
    regex_array[loop] = finish;
    return regex_array;
}

My_Regex *handle_group(size_t pattern_index, const char *pattern)
{
    size_t group_size = get_group_size(pattern_index, pattern);
    char *pGroup = malloc(sizeof(char) * group_size - 1); // create space for group // -1 because we need '\0' at the end
    if (pGroup == NULL)
        return NULL; // TODO: Handle in upper function

    My_Regex *pRegex = malloc(sizeof(My_Regex));
    if (pRegex == NULL)
        return NULL; // TODO: Handle in upper function

    pRegex->regex = pGroup;
    pRegex->is_regex_group = true;
    pRegex->is_prohibition_group = false;

    ++pattern_index;
    char c = pattern[pattern_index];
    if (c == '^')
    {
        pRegex->is_prohibition_group = true;
        ++pattern_index;
    }

    size_t group_index = 0;
    for (; group_index < group_size - 2; ++pattern_index, ++group_index)
    {
        pGroup[group_index] = pattern[pattern_index];
    }
    pGroup[group_index + 1] = '\0';
    return pRegex;
}

size_t get_group_size(size_t pattern_index, const char *pattern)
{
    size_t size = 0;
    char c;
    do
    {
        c = pattern[pattern_index];
        if (c == '^') // TODO : possible error
            --size;
        ++pattern_index;
        ++size;
    } while (c != ']' && c != '\0');
    if (c == '\0')
        return 0; // not a group
    return size;
}

size_t get_pattern_real_size(const char *pattern)
{
    size_t current_location = 0;
    size_t real_size = 0;
    char c;
    do
    {
        c = pattern[current_location];
        if (c == '[')
        {
            size_t group_indicator_found_at = current_location;
            do
            {
                c = pattern[current_location];
                ++current_location;
            } while (c != ']' && c != '\0');
            if (c == '\0')
            {
                real_size += current_location - group_indicator_found_at; // deal with group start with no group end
                --real_size;                                              // dont count \0
                return real_size;
            }
            --current_location;
        }
        ++real_size;
        ++current_location;
    } while (c != '\0');
    --real_size; // count for \0
    return real_size;
}