#ifndef GC
#define GC

#ifndef STDLIBH
#define STDLIBH
#include <stdlib.h>
#include <stdio.h>
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

/// @brief Frees one specified object
/// @param target Object to free
void free_one_object(void *target);

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
        free_one_object(source); // possible data loss
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

void free_one_object(void *target)
{
    size_t obj_id = find_malloced_obj_id(target);
    if (obj_id == MALLOCED_OJB_ARR_SIZE)
    {
        printf("Error while freeing object - obj not in array");
        exit(-11);
    }
    if (Objects_to_free[obj_id] == NULL)
    {
        printf("Error while freeing object - obj already freed");
        exit(-11);
    }
    free(Objects_to_free[obj_id]);
    Objects_to_free[obj_id] = NULL;
}

void free_objects()
{
    for (size_t i = 0; i < used_object_to_free_id; ++i)
    {
        if (Objects_to_free[i] == NULL)
            continue;
        free(Objects_to_free[i]);
    }
    free(Objects_to_free);
}
#endif

#include "graph.h"
#include <stdbool.h>
#include <stdio.h>

#define START_BUF_SIZE 1

size_t graph_count = 0;

graph_t *allocate_graph()
{
    graph_t *target = handled_malloc(sizeof(graph_t) * 1);
    edge_t *edges = NULL;
    int edges_count = 0;

    target->edges = edges;
    target->num_edges = edges_count;

    ++graph_count;

    return target;
}

void free_graph(graph_t **graph)
{
    graph_t *g = graph[0];
    free_one_object(g->edges);
    free_one_object(g);
    --graph_count;
    if (graph_count == 0)
    {
        free_objects();
    }
}

void load_txt(const char *fname, graph_t *graph)
{

    FILE *f = fopen(fname, "r");
    if (f == NULL)
    {
        printf("File not found: load_txt %s", fname);
        exit(-11);
    }

    size_t used = 0;
    size_t buf_size = START_BUF_SIZE;
    edge_t *buffer = handled_malloc(sizeof(edge_t) * buf_size);
    while (!feof(f))
    {
        if (used == buf_size - 1)
        {
            buf_size *= 2;
            buffer = handled_realloc(buffer, sizeof(edge_t) * buf_size);
        }
        int start = get_next_int_from_line(f);
        int end = get_next_int_from_line(f);
        int weight = get_next_int_from_line(f);

        if (start == 0 && end == 0 && weight == 0)
        {
            break;
        }

        buffer[used].from = start;
        buffer[used].to = end;
        buffer[used].cost = weight;
        ++used;
    }
    graph->edges = buffer;
    graph->num_edges = used;
    fclose(f);
}

bool is_line_empty()
{
    return false;
}

int get_next_int_from_line(FILE *f)
{
    char temp[11] = {0};

    char c;
    int used = 0;
    do
    {
        c = fgetc(f);
        if (c == ' ' || c == '\n' || c == EOF)
        {
            break;
        }
        temp[used] = c;
        ++used;
    } while (c != ' ' && c != '\n');
    temp[used] = '\0';
    int target = atoi(temp);
    return target;
}

void load_bin(const char *fname, graph_t *graph)
{
    FILE *f = fopen(fname, "rb");
    if (f == NULL)
    {
        printf("File not found: load_bin %s", fname);
        exit(-11);
        // TODO : Handle non existing file
    }

    size_t used = 0;
    size_t buf_size = START_BUF_SIZE;
    edge_t *buffer = handled_malloc(sizeof(edge_t) * buf_size);
    while (!feof(f))
    {
        if (used == buf_size - 1)
        {
            buf_size *= 2;
            buffer = handled_realloc(buffer, sizeof(edge_t) * buf_size);
        }
        int start = read_int(f);
        int end = read_int(f);
        int weight = read_int(f);
        if (feof(f))
        {
            break;
        }
        buffer[used].from = start;
        buffer[used].to = end;
        buffer[used].cost = weight;
        ++used;
    }
    graph->edges = buffer;
    graph->num_edges = used;
    fclose(f);
}

int read_int(FILE *f)
{
    char byte1;
    char byte2;
    char byte3;
    char byte4;

    fread(&byte1, 1, 1, f);
    fread(&byte2, 1, 1, f);
    fread(&byte3, 1, 1, f);
    fread(&byte4, 1, 1, f);

    char tmp[4] = {byte4, byte3, byte2, byte1};
    int *fun = (int *)tmp;
    return *fun;
}

void save_graph_text(FILE *f, const graph_t *const graph)
{
    for (size_t i = 0; i < graph->num_edges; ++i)
    {
        char temp[10] = {0};
        int start = graph->edges[i].from;
        intToStr(start, temp, 10);
        print_it(f, temp);
        fputc(' ', f);

        char temp2[10] = {0};
        int end = graph->edges[i].to;
        intToStr(end, temp2, 10);
        print_it(f, temp2);
        fputc(' ', f);

        char temp3[10] = {0};
        int weight = graph->edges[i].cost;
        intToStr(weight, temp3, 10);
        print_it(f, temp3);
        fputc('\n', f);
    }
}

void intToStr(int num, char *str, int buf_size)
{
    int i = 0, isNegative = 0;

    // Handle negative numbers
    if (num < 0)
    {
        isNegative = 1;
        num = -num;
    }

    // Calculate the number of digits in the number
    int temp = num;
    int digits = 0;
    do
    {
        ++digits;
        temp /= 10;
    } while (temp != 0);

    if (digits > buf_size - 1)
    {
        // TODO: handle buffer overflow
    }

    // If negative, leave space for the '-' sign
    if (isNegative)
    {
        str[0] = '-';
        i++;
    }

    // Build the string in the correct order
    for (int j = digits - 1; j >= 0; --j)
    {
        str[i + j] = (num % 10) + '0';
        num /= 10;
    }

    // Null-terminate the string
    str[i + digits] = '\0';
}

void print_it(FILE *f, char *str)
{
    for (size_t i = 0; str[i] != '\0'; ++i)
    {
        fputc(str[i], f);
    }
}

void save_txt(const graph_t *const graph, const char *fname)
{
    FILE *out = fopen(fname, "w");
    if (out == NULL)
    {
        // TODO: Handle not found file
    }
    save_graph_text(out, graph);
    fclose(out);
}

void save_bin(const graph_t *const graph, const char *fname)
{
    FILE *f = fopen(fname, "wb");
    for (size_t i = 0; i < graph->num_edges; ++i)
    {
        edge_t edge = graph->edges[i];
        int start = edge.from;
        int end = edge.to;
        int weight = edge.cost;

        write_int(start, f);
        write_int(end, f);
        write_int(weight, f);
    }
    fclose(f);
}

void write_int(int source, FILE *file)
{
    char *p2 = (char *)&source;
    char byte1 = *(p2);
    char byte2 = *(p2 + 1);
    char byte3 = *(p2 + 2);
    char byte4 = *(p2 + 3);

    fwrite(&byte4, sizeof(char), 1, file);
    fwrite(&byte3, sizeof(char), 1, file);
    fwrite(&byte2, sizeof(char), 1, file);
    fwrite(&byte1, sizeof(char), 1, file);
}