#ifndef GC
#define GC

#ifndef STDLIBH
#define STDLIBH
#include <stdlib.h>
#endif

#define MALLOCED_OJB_ARR_SIZE 1000

void **Objects_to_free;
size_t size_objects_to_free = 0;
size_t used_object_to_free_id = 0;

/// @brief Uses malloc to get pointer for heap, creates an entry into Objects_to_free and saves pointer there
/// @param size Size of object
/// @return Pointer
void *handled_malloc(size_t size);

/// @brief Looks for source in Objects_to_free to get the id, reallocs and saves new pointer to Objects_to_free
/// @param source Pointer to realloc
/// @param size Size of new malloc
/// @return New pointer
void *handled_realloc(void *source, size_t size);

/// @brief Finds entry in array
/// @param source
/// @return
size_t find_malloced_obj_id(void *source);

/// @brief Frees objects in Objects_to_free
void free_objects();

void *handled_malloc(size_t size)
{
    if (size_objects_to_free == 0)
    {
        size_objects_to_free = MALLOCED_OJB_ARR_SIZE;
        void **temp = malloc(sizeof(void *) * size_objects_to_free);
        if (temp == NULL)
        {
            return NULL;
        }
        Objects_to_free = temp;
    }
    if (size_objects_to_free < used_object_to_free_id + 2)
    {
        size_objects_to_free *= 2;
        void **temp = realloc(Objects_to_free, size_objects_to_free * sizeof(void *));
        if (temp == NULL)
        {
            return NULL;
        }
        Objects_to_free = temp;
    }
    void *temp = malloc(size);
    if (temp == NULL)
    {
        return NULL;
    }
    Objects_to_free[used_object_to_free_id] = temp;
    ++used_object_to_free_id;
    return temp;
}

void *handled_realloc(void *source, size_t size)
{
    size_t obj_id = find_malloced_obj_id(source);
    void *temp = realloc(source, size);
    if (temp == NULL)
    {
        free(source); // possible data loss
    }
    Objects_to_free[obj_id] = temp;
    return temp;
}

size_t find_malloced_obj_id(void *source)
{
    for (size_t i = 0; i < MALLOCED_OJB_ARR_SIZE; ++i)
    {
        if (source == Objects_to_free[i])
            return i;
    }
    return MALLOCED_OJB_ARR_SIZE; // TODO: check for problems
}

void free_objects()
{
    for (size_t i = 0; i < used_object_to_free_id; ++i)
    {
        free(Objects_to_free[i]);
    }
    free(Objects_to_free);
}
#endif

// ------------------------- Main -----------------------------------

#include <stdio.h>
#include <string.h>

#define DATABASE_SIZE 100
#define DATABASE_DIR "./database/"
#define DATABASE_FORMAT ".dat"
#define CURRENT_YEAR 2024

// make struct
typedef struct
{
    char *name;
    char *address;
    char *phone;
    char *DOB; // date of birth
    char *occupation;
    int age_in_twenty_twentyfour;
    int salary;
} data_entry_t;

typedef struct
{
    int age; // suppose int is enough
    char *date;
} options_t;

data_entry_t *read_data();

data_entry_t get_data_from_file(FILE *target);

char *remove_metadata(const char *meta, char *data);

char *read_line(FILE *f);

int get_digit_count(int num);

int get_age_from_DOB(char *DOB, int year);

// read in files

int main(int argc, char **argv)
{

    data_entry_t *data_list = read_data();
    printf("%s age: %i", data_list[0].name);
    free_objects();
    return 1;
}

data_entry_t *read_data()
{
    int buffer_size = strlen(DATABASE_DIR) + get_digit_count(DATABASE_SIZE) + 4 + 1; // null terminator 1
    char f_name[buffer_size];
    data_entry_t *target = handled_malloc(DATABASE_SIZE * sizeof(data_entry_t));
    for (size_t i = 1; i <= DATABASE_SIZE; ++i)
    {
        int offset = get_digit_count(i) + strlen(DATABASE_DIR) + strlen(DATABASE_FORMAT);
        int file_padding = buffer_size - offset;
        if (file_padding < 0)
            file_padding = 0;
        if (i > 9)
            file_padding += 1; // i have no idea what is going on here, but this works if i is more than 1 digit
        sprintf(f_name, "%s%0*lu%s", DATABASE_DIR, file_padding, i, DATABASE_FORMAT);
        FILE *f = fopen(f_name, "r");
        if (f == NULL)
            continue;
        data_entry_t data = get_data_from_file(f);
        fclose(f);
        target[i - 1] = data;
    }
    return target;
}

data_entry_t get_data_from_file(FILE *target)
{
    data_entry_t tmp;
    char *name = remove_metadata("Name: ", read_line(target));
    char *address = remove_metadata("Address: ", read_line(target));
    char *phone = remove_metadata("Phone: ", read_line(target));
    char *DOB = remove_metadata("Date of birth: ", read_line(target));
    char *occupation = remove_metadata("Occupation: ", read_line(target));
    char *salary_non_clean = remove_metadata("Salary: ", read_line(target));
    int salary = atoi(salary_non_clean);
    tmp.name = name;
    tmp.address = address;
    tmp.phone = phone;
    tmp.DOB = DOB;
    tmp.occupation = occupation;
    tmp.salary = salary;
    tmp.age_in_twenty_twentyfour = get_age_from_DOB(DOB, CURRENT_YEAR);
    return tmp;
}

char *remove_metadata(const char *meta, char *data)
{
    return &data[strlen(meta)]; // TODO: Ew
}

char *read_line(FILE *f)
{
    size_t buff_size = 10;
    size_t used = 0;
    char *buffer = handled_malloc(buff_size * sizeof(char));
    char c;
    do
    {
        if (used == buff_size - 1)
        {
            buff_size += 10;
            buffer = handled_realloc(buffer, buff_size);
        }
        c = fgetc(f);
        if (c == EOF || c == '\n')
        {
            buffer[used] = '\0';
            break;
        }
        buffer[used] = c;
        ++used;
    } while (c != EOF && c != '\n');
    return buffer;
}

int get_digit_count(int num)
{
    int count = 0;

    while (num > 0)
    {
        count++;
        num = num / 10;
    }
    return count;
}

int get_age_from_DOB(char *DOB, int year)
{
    char c[5];
    for (size_t i = 0; i < 4; ++i)
    {
        c[i] = DOB[i];
    }
    c[4] = '\0';
    int year_from_data = atoi(c);
    return year - year_from_data;
}
