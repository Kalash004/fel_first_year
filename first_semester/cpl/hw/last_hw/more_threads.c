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
    source = NULL;
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
        if (Objects_to_free[i] != NULL)
        {
            free(Objects_to_free[i]);
            Objects_to_free[i] = NULL;
        }
    }
    free(Objects_to_free);
    Objects_to_free = NULL;
}
#endif
// --------------------------- Garbage collector end -----------------------------

// ------------------------- Main -----------------------------------

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include "queue.h"

#define DATABASE_SIZE 100
#define DATABASE_DIR "./database/"
#define DATABASE_FORMAT ".dat"
#define CURRENT_YEAR 2024
#define PRODUCER_COUNT 2
#define CONSUMER_COUNT 2
#define BUFFER_SIZE 100

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

typedef struct
{
    size_t db_start;
    size_t db_end;
    bool *stop_flag;
    queue_t *q;

} producer_args_t;

typedef struct
{
    queue_t *queue;
    stats_t *target_stats;
    data_entry_t *target_data;
    options_t options;
    bool *stop_flag;
} consumer_args_t;

void display_output(stats_t stats, data_entry_t data);

data_entry_t get_data_from_file(FILE *target);

char *remove_metadata(const char *meta, char *data);

char *read_line(FILE *f);

int get_digit_count(int num);

options_t parse_args(int argc, char **argv);

void DOB_to_int(char *DOB, int *day, int *month, int *year_c);

void print_data(data_entry_t target);

void add_month_count(data_entry_t data, stats_t *save_stats_to);

void print_histogram(stats_t stats);

void print_stats(access_stats_u access);

void print_stat(int padding, int num);

void print_month_abbreviations(access_stats_u access);

void print_month_abbreviation(int padding, int month_id);

void print_border(access_stats_u access);

void *producer_handler(void *args_input);

void *consumer_handler(void *args_input);

void initialize_stats_to_zero(stats_t *stats);

data_entry_t get_one_entry(size_t index);

void make_file_name(size_t index, char *target, int size);

bool is_needed_entry_count_stat(data_entry_t data, options_t opt, stats_t *save_stats_to);

// ------------------------ header end

pthread_mutex_t queue_push_lock;
pthread_mutex_t queue_capacity_check_lock;
pthread_mutex_t data_counter_lock;

size_t left_to_process_count = DATABASE_SIZE;
size_t chunk_size = DATABASE_SIZE / PRODUCER_COUNT;

int main(int argc, char **argv)
{
    options_t opt = parse_args(argc, argv);

    bool stop_flag = false;
    pthread_mutex_init(&queue_push_lock, NULL);
    pthread_mutex_init(&queue_capacity_check_lock, NULL);
    pthread_mutex_init(&data_counter_lock, NULL);

    stats_t found_stats = {0};
    data_entry_t found_data = {0};

    queue_t *data_queue = create_queue(BUFFER_SIZE);

    // make pool of producers
    pthread_t producer_pool[PRODUCER_COUNT];
    for (size_t i = 0; i < PRODUCER_COUNT; ++i)
    {
        producer_args_t *arg_producers = handled_malloc(sizeof(producer_args_t));
        arg_producers->db_start = i * chunk_size + 1;
        arg_producers->db_end = (i == PRODUCER_COUNT - 1) ? DATABASE_SIZE : arg_producers->db_start + chunk_size - 1;
        arg_producers->q = data_queue;
        arg_producers->stop_flag = &stop_flag;

        pthread_t t;
        if (pthread_create(&t, NULL, producer_handler, arg_producers) != 0)
        {
            fprintf(stderr, "Error creating producer thread %zu\n", i);
            return -1;
        }
        producer_pool[i] = t;
    }

    // make pool of consumers
    pthread_t consumer_pool[CONSUMER_COUNT];
    consumer_args_t *arg_consumers = handled_malloc(sizeof(consumer_args_t));
    arg_consumers->options = opt;
    arg_consumers->queue = data_queue;
    arg_consumers->stop_flag = &stop_flag;
    arg_consumers->target_data = &found_data;
    arg_consumers->target_stats = &found_stats;
    for (size_t i = 0; i < CONSUMER_COUNT; ++i)
    {
        pthread_t t;
        if (pthread_create(&t, NULL, consumer_handler, arg_consumers) != 0)
        {
            fprintf(stderr, "Error creating consumer thread %zu\n", i);
            return -1;
        }
        consumer_pool[i] = t;
    }

    // await for producers to stop
    for (size_t i = 0; i < PRODUCER_COUNT; ++i)
    {
        pthread_join(producer_pool[i], NULL);
    }

    // await for consumers to stop
    printf("Waiting for consumers to finish\n");
    for (size_t i = 0; i < CONSUMER_COUNT; ++i)
    {
        pthread_join(consumer_pool[i], NULL);
    }

    display_output(found_stats, found_data);
    delete_queue(data_queue);
    free_objects();
    return 0;
}

void *producer_handler(void *args_input)
{
    // Unpack arguments
    producer_args_t args = *(producer_args_t *)args_input;
    size_t index = args.db_start;
    while (!*args.stop_flag)
    {

        if (index == args.db_end + 1)
        {
            break;
        }

        data_entry_t *data_holder = handled_malloc(sizeof(data_entry_t));
        data_entry_t data = get_one_entry(index);
        *data_holder = data;

        pthread_mutex_lock(&queue_capacity_check_lock);
        while (args.q->size == args.q->capacity)
        {
            sleep(5);
        }
        pthread_mutex_lock(&queue_push_lock);
        push_to_queue(args.q, data_holder);
        pthread_mutex_unlock(&queue_push_lock);
        pthread_mutex_unlock(&queue_capacity_check_lock);
        ++index;
    }
    pthread_mutex_lock(&data_counter_lock);
    pthread_mutex_lock(&queue_push_lock);
    left_to_process_count -= chunk_size;
    pthread_mutex_unlock(&queue_push_lock);
    pthread_mutex_unlock(&data_counter_lock);
    return NULL;
}

void *consumer_handler(void *args_input)
{
    consumer_args_t args = *(consumer_args_t *)args_input;
    stats_t stats = {0};
    data_entry_t data = {0};
    size_t j = 1;
    while (!*args.stop_flag)
    {

        if (left_to_process_count <= 0)
        {
            *args.stop_flag = true;
            data.birth_day = 0;
            data.birth_month = 0;
            data.birth_year = 0;
            *args.target_data = data;
            break;
        }

        pthread_mutex_lock(&queue_push_lock);

        if (args.queue->size == 0) // Checking if queue is empty
        {
            pthread_mutex_unlock(&queue_push_lock);
            continue;
        }
        void *temp = pop_from_queue(args.queue);
        if (temp == NULL)
        {
            printf("Caught null");
            *args.stop_flag = true;
        }
        data_entry_t *temp_typed = (data_entry_t *)temp;
        pthread_mutex_unlock(&queue_push_lock);
        data = *temp_typed;

        bool is_found = is_needed_entry_count_stat(*temp_typed, args.options, &stats);

        if (is_found)
        {
            *args.stop_flag = true;
            *args.target_data = data;
            *args.target_stats = stats;
            args.target_stats->avg_salary = stats.sum_salary / j;
            return NULL;
        }
        ++j;
    }

    return NULL;
}

void initialize_stats_to_zero(stats_t *stats)
{
    if (stats != NULL)
    {
        memset(stats, 0, sizeof(stats_t));
    }
}

void display_output(stats_t stats, data_entry_t data)
{
    if (data.birth_day == 0 && data.birth_month == 0 && data.birth_year == 0)
    {
        printf("Not found\n");
        return;
    }
    printf("#Average salary: %i\n", stats.avg_salary);
    print_histogram(stats);
    printf("#Found entry:\n");
    print_data(data);
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

options_t parse_args(int argc, char **argv)
{
    if (argc != 3)
    {
        // TODO: Handle argcount;
        printf("Bad amount of args\n");
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

bool is_needed_entry_count_stat(data_entry_t data, options_t opt, stats_t *save_stats_to)
{
    save_stats_to->sum_salary += data.salary;
    add_month_count(data, save_stats_to);
    if (data.age_in_twenty_twentyfour != opt.age)
        return false;
    if (data.birth_month != opt.month || data.birth_day != opt.day)
        return false;
    return true;
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
