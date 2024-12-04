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