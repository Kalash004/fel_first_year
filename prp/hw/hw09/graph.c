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

graph_t *allocate_graph()
{
    graph_t *target = handled_malloc(sizeof(graph_t) * 1);
    int *points = NULL;
    int points_count = 0;
    edge_t **edges = NULL;
    int edges_count = 0;

    target->edges = edges;
    target->edges_count = edges_count;

    return target;
}

void free_graph(graph_t **graph)
{
    graph_t *g = *graph;
    free_one_object(g->edges);
    free_one_object(g);
}

void load_txt(const char *fname, graph_t *graph)
{
    /**
start end weight
0 1 7
0 2 9
0 5 14
1 2 10
1 3 15
2 3 11
2 5 255
3 4 20001
4 5 9
     */

    FILE *f = fopen(fname, "r");
    if (f == NULL)
    {
        printf("File not found: load_txt %s", fname);
        exit(-11);
    }
    while (!feof(f))
    {
        edge_t *new_edge = handled_malloc(sizeof(edge_t) * 1);
        int start = get_next_int_from_line(f);
        int end = get_next_int_from_line(f);
        int weight = get_next_int_from_line(f);
    }

}

int get_next_int_from_line(FILE *f)
{
    char temp[10] = {};

    char c;
    int used = 0;
    do
    {
        c = fgetc(f);
        if (c == ' ' || c == '\n')
            break;
        temp[used] = c;
        ++used;
    } while (c != ' ' && c != '\n');
    int target = atoi(temp);
    return target;
}