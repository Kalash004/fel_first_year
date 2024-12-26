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
// --------------------------- Garbage collector end -----------------------------

// ------------------------- Main -----------------------------------

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h"

#define DATABASE_SIZE 100
#define DATABASE_DIR "./database/"
#define DATABASE_FORMAT ".dat"
#define CURRENT_YEAR 2024
#define PRODUCER_COUNT 1
#define CONSUMER_COUNT 1
#define BUFFER_SIZE 30

// make struct
typedef struct
{
    char *name;
    char *address;
    char *phone;
    int birth_year;
    int birth_month;
    int birth_day;
    char *occupation;
    int age_in_twenty_twentyfour; // suppose int is enough
    int salary;                   // suppose int is enough
} data_entry_t;

typedef struct
{
    int age; // suppose int is enough
    int day;
    int month;
} options_t;

typedef struct
{
    long long int sum_salary;
    int avg_salary;
    int jan_count;
    int feb_count;
    int mar_count;
    int apr_count;
    int may_count;
    int jun_count;
    int jul_count;
    int aug_count;
    int sep_count;
    int oct_count;
    int nov_count;
    int dec_count;
} stats_t;

typedef union
{
    stats_t stats_u;
    int arr[sizeof(stats_t) / sizeof(int)];

} access_stats_u;

void display_output(stats_t stats, data_entry_t data);

data_entry_t *get_data_data_list(size_t start, size_t end);

data_entry_t get_data_from_file(FILE *target);

char *remove_metadata(const char *meta, char *data);

char *read_line(FILE *f);

int get_digit_count(int num);

int get_age_from_DOB(char *DOB, int year);

options_t parse_args(int argc, char **argv);

void DOB_to_int(char *DOB, int *day, int *month, int *year_c);

bool compare_month_day(char *x, char *y);

data_entry_t find_needed_entry_count_stats(data_entry_t *data_list, options_t opt, stats_t *save_stats_to);

data_entry_t get_empty_data_entry();

void print_data(data_entry_t target);

void add_month_count(data_entry_t data, stats_t *save_stats_to);

void print_histogram(stats_t stats);

void print_stats(access_stats_u access);

void print_stat(int padding, int num);

void print_month_abbreviations(access_stats_u access);

void print_month_abbreviation(int padding, int month_id);

void print_border(access_stats_u access);

int find_most_digit_number(stats_t stats);

// read in files

int main(int argc, char **argv)
{
    options_t opt = parse_args(argc, argv);
    stats_t found_stats = {0};
    data_entry_t found_data = {0};

    queue_t *data_queue = create_queue(BUFFER_SIZE);

    // make pool of producers
    pthread_t producer_pool[PRODUCER_COUNT];
    for (size_t i = 0; i < PRODUCER_COUNT; ++i)
    {
        // producer_pool[i] = pthread_create();
    }

    // make pool of consumers
    pthread_t consumer_pool[CONSUMER_COUNT];
    for (size_t i = 0; i < CONSUMER_COUNT; ++i)
    {
        // consumer_pool[i] = pthread_create();
    }

    // get data to consumers

    // get found entry and stats back

    display_output(found_stats, found_data);
    free_objects();
    return 0;
}

void producer_handler(size_t db_start, size_t db_end, bool *stop_flag, queue_t *q)
{
    size_t index = db_start;
    while (!stop_flag)
    {
        if (index > DATABASE_SIZE)
        {
            *stop_flag = true;
            break;
        }
        // get data
        data_entry_t *data_holder = handled_malloc(sizeof(data_entry_t));
        data_entry_t data = get_one_entry(index);
        *data_holder = data;
        // check queue not full
        if (q->size == q->capacity)
        {
            // wait ?
        }
        // push into queue
        place_data_into_queue(data_holder, q);
        ++index;
    }
}

bool place_data_into_queue(void *data, queue_t *q)
{
    push_to_queue(q, data);
}

void display_output(stats_t stats, data_entry_t data)
{
    printf("#Average salary: %i\n", stats.avg_salary);
    print_histogram(stats);
    if (data.birth_day == 0 && data.birth_month == 0 && data.birth_year == 0)
    {
        printf("Not found");
    }
    else
    {
        printf("#Found entry:\n");
        print_data(data);
    }
}

data_entry_t *get_data_data_list(size_t start, size_t end)
{
    int buffer_size = strlen(DATABASE_DIR) + get_digit_count(DATABASE_SIZE) + 4 + 1; // null terminator 1
    char f_name[buffer_size];
    data_entry_t *target = handled_malloc(DATABASE_SIZE * sizeof(data_entry_t));
    for (size_t i = start; i <= end; ++i)
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

data_entry_t get_one_entry(size_t index)
{
    data_entry_t target = {0};
    int buffer_size = strlen(DATABASE_DIR) + get_digit_count(DATABASE_SIZE) + 4 + 1; // null terminator 1
    char f_name[buffer_size];
    make_file_name(index, f_name, buffer_size);
    FILE *f = fopen(f_name, "r");
    if (f == NULL)
        return target;
    data_entry_t data = get_data_from_file(f);
    fclose(f);
    target = data;
    return target;
}

void make_file_name(size_t index, char *target, int size)
{
    int offset = get_digit_count(index) + strlen(DATABASE_DIR) + strlen(DATABASE_FORMAT);
    int file_padding = size - offset;
    if (file_padding < 0)
        file_padding = 0;
    if (index > 9)
        file_padding += 1; // i have no idea what is going on here, but this works if i is more than 1 digit
    sprintf(target, "%s%0*lu%s", DATABASE_DIR, file_padding, index, DATABASE_FORMAT);
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
    DOB_to_int(DOB, &tmp.birth_day, &tmp.birth_month, &tmp.birth_year);
    tmp.name = name;
    tmp.address = address;
    tmp.phone = phone;
    tmp.occupation = occupation;
    tmp.salary = salary;
    tmp.age_in_twenty_twentyfour = CURRENT_YEAR - tmp.birth_year;
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

options_t parse_args(int argc, char **argv)
{
    if (argc != 3)
    {
        // TODO: Handle argcount;
        printf("Bad amount of args");
        exit(1);
    }
    options_t target;
    target.age = atoi(argv[1]);

    char day_c[3], month_c[3];

    day_c[0] = (argv[2])[0];
    day_c[1] = (argv[2])[1];
    day_c[2] = '\0';

    month_c[0] = argv[2][3];
    month_c[1] = argv[2][4];
    month_c[2] = '\0';

    target.day = atoi(day_c);
    target.month = atoi(month_c);
    return target;
}

void DOB_to_int(char *DOB, int *day, int *month, int *year)
{
    char day_c[3], month_c[3], year_c[5];

    day_c[0] = DOB[8];
    day_c[1] = DOB[9];
    day_c[2] = '\0';

    month_c[0] = DOB[5];
    month_c[1] = DOB[6];
    month_c[2] = '\0';

    year_c[0] = DOB[0];
    year_c[1] = DOB[1];
    year_c[2] = DOB[2];
    year_c[3] = DOB[3];
    year_c[4] = '\0';

    *day = atoi(day_c);
    *month = atoi(month_c);
    *year = atoi(year_c);
}

bool compare_month_day(char *x, char *y)
{
    // Temporary arrays to store month and day substrings
    char month1[3], day1[3];
    char month2[3], day2[3];

    // Extracting substrings for month and day from input strings
    month1[0] = x[5];
    month1[1] = x[6];
    month1[2] = '\0';

    day1[0] = x[8];
    day1[1] = x[9];
    day1[2] = '\0';

    month2[0] = y[5];
    month2[1] = y[6];
    month2[2] = '\0';

    day2[0] = y[8];
    day2[1] = y[9];
    day2[2] = '\0';

    // Converting extracted substrings to integers
    int month1_int = atoi(month1);
    int day1_int = atoi(day1);
    int month2_int = atoi(month2);
    int day2_int = atoi(day2);
    return (month1_int == month2_int && day1_int == day2_int) ? true : false;
}

data_entry_t find_needed_entry_count_stats(data_entry_t *data_list, options_t opt, stats_t *save_stats_to)
{
    data_entry_t target = get_empty_data_entry();
    data_entry_t data;
    size_t i;
    for (i = 0; i < DATABASE_SIZE; ++i)
    {
        data = data_list[i];
        save_stats_to->sum_salary += data.salary;
        add_month_count(data, save_stats_to);
        if (data.age_in_twenty_twentyfour != opt.age)
            continue;
        if (data.birth_month != opt.month || data.birth_day != opt.day)
            continue;
        target = data;
        break;
    }
    save_stats_to->avg_salary = save_stats_to->sum_salary / (i + 1);
    return target;
}

data_entry_t get_empty_data_entry()
{
    data_entry_t data;
    data.address = "";
    data.age_in_twenty_twentyfour = 0;
    data.birth_day = 0;
    data.birth_month = 0;
    data.birth_year = 0;
    data.occupation = "";
    data.phone = "";
    data.salary = -1;
    data.name = "";
    return data;
}

void print_data(data_entry_t target)
{
    char month[3], day[3];
    sprintf(month, "%0*i", 3 - get_digit_count(target.birth_month), target.birth_month);
    sprintf(day, "%0*i", 3 - get_digit_count(target.birth_day), target.birth_day);
    printf("Name: %s\nAddress: %s\nPhone: %s\nDate of birth: %i-%s-%s\nOccupation: %s\nSalary: %i\n", target.name, target.address, target.phone, target.birth_year, month, day, target.occupation, target.salary);
}

void add_month_count(data_entry_t data, stats_t *save_stats_to)
{
    switch (data.birth_month)
    {
    case 1:
        save_stats_to->jan_count += 1;
        break;
    case 2:
        save_stats_to->feb_count += 1;
        break;
    case 3:
        save_stats_to->mar_count += 1;
        break;
    case 4:
        save_stats_to->apr_count += 1;
        break;
    case 5:
        save_stats_to->may_count += 1;
        break;
    case 6:
        save_stats_to->jun_count += 1;
        break;
    case 7:
        save_stats_to->jul_count += 1;
        break;
    case 8:
        save_stats_to->aug_count += 1;
        break;
    case 9:
        save_stats_to->sep_count += 1;
        break;
    case 10:
        save_stats_to->oct_count += 1;
        break;
    case 11:
        save_stats_to->nov_count += 1;
        break;
    case 12:
        save_stats_to->dec_count += 1;
        break;
    default:
        // TODO: Handle something bad
        break;
    }
}

void print_histogram(stats_t stats)
{
    access_stats_u access;
    access.stats_u = stats;

    printf("#Month histogram:\n");
    print_border(access);
    print_month_abbreviations(access);
    print_border(access);
    print_stats(access);
    print_border(access);
}

void print_stats(access_stats_u access)
{
    for (size_t i = 3; i < sizeof(stats_t) / sizeof(int) - 1; ++i)
    {
        int num = access.arr[i];
        int padding = get_digit_count(num) + 2;
        padding = (padding < 5) ? 5 : padding;
        print_stat(padding, num);
    }
    printf("|\n");
}

void print_stat(int padding, int num)
{
    printf("|%*i ", padding - 1, num);
}

void print_month_abbreviations(access_stats_u access)
{
    for (size_t i = 3; i < sizeof(stats_t) / sizeof(int) - 1; ++i)
    {
        int num = access.arr[i];
        int padding = get_digit_count(num) + 2;
        padding = (padding < 5) ? 5 : padding;
        print_month_abbreviation(padding, i - 3);
    }
    printf("|\n");
}

void print_month_abbreviation(int padding, int month_id)
{
    char *months[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    printf("| %*s ", padding - 2, months[month_id]);
}

void print_cell_border(int width_per_cell)
{
    printf("+");
    for (size_t i = 0; i < width_per_cell; ++i)
    {
        printf("-");
    }
}

void print_border(access_stats_u access)
{
    for (size_t i = 3; i < sizeof(stats_t) / sizeof(int) - 1; ++i)
    {
        int num = access.arr[i];
        int padding = get_digit_count(num) + 2;
        padding = (padding < 5) ? 5 : padding;
        print_cell_border(padding);
    }
    printf("+\n");
}
