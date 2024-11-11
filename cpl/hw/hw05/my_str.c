#include "./my_str.h"
#include <stdlib.h>
#include <stdbool.h>

size_t _get_str_size(const char *_source);
void fill_target_with_token(char *temp, size_t token_start, size_t token_end, char *str);
size_t find_next_delim_end(char *str, const char *delim, size_t *str_loc, size_t delim_size, bool *done_flag, bool *delim_found_flag);
size_t get_delim_size(const char *delim);
int is_char_in_str(char c, const char *string);
size_t get_first_non_delim_char_location(char *str, const char *delim, size_t delim_size);

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