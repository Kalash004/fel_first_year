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




/**

 */
#include "simage.h" 
#include <string.h>

typedef struct
{
    char *in_file;
    char *out_file;
    char **operations;
    size_t oper_count;
} args_t;


/// @brief Parses arguments from command line into args_t
/// @param argc arg count int
/// @param argv array of strings (args)
/// @return args_t allocated
args_t *parse_args(int argc, char *argv[]);

int check_file_bmp(char *file_name);

void handle_operations(args_t *args, SImage* image);

void handle_operation(char *oper, SImage* image);

SImage* shrink(SImage *image);

SImage* flip(SImage *image);

SImage* enlarge(SImage *image);

SImage* rotate(SImage *image);

void print_args(args_t* args);

int main(int argc, char *argv[]) {
    args_t* args = parse_args(argc, argv);
    if (args == NULL) {
        // TODO: handle
        return 1;
    }
    print_args(args);
    SImage *img = handled_malloc(sizeof(SImage) * 1);
    loadImage(img, args->in_file);
    handle_operations(args, img);
    saveImage(img, args->out_file);
    // garbage collects
    free_objects();
}

args_t *parse_args(int argc, char *argv[]) {
    if (argc < 4) {
        // TODO: Handle exception
        return NULL;
    }
    args_t *target = (args_t *)handled_malloc(sizeof(args_t) * 1);
    if (check_file_bmp(argv[1])) {
        // TODO: Handle exception
        return NULL;
    }
    if (check_file_bmp(argv[2])) {
        // TODO: Handle exception
        return NULL;
    }
    target->in_file = argv[1];
    target->out_file = argv[2];
    target->oper_count = argc - 3;
    target->operations = handled_malloc(sizeof(char *) * target->oper_count);
    int id = 0;
    for (int i = 3; i < argc; ++i) {
        char *oper = handled_malloc(sizeof(char) * 2);
        oper[0] = argv[i][0];
        oper[1] = '\0';
        target->operations[id] = oper;
        id++;
    }

    return target;
}

int check_file_bmp(char *file_name) {
    int size = strlen(file_name) + 1;
    if (file_name[size - 2] != 'p')
        return 1;
    if (file_name[size - 3] != 'm')
        return 1;
    if (file_name[size - 4] != 'b')
        return 1;
    if (file_name[size - 5] != '.')
        return 1;
    return 0;
}

void handle_operations(args_t *args, SImage *image) {
    for (size_t i = 0; i < args->oper_count; ++i) {
        handle_operation(args->operations[i], image);
    }
}

void handle_operation(char *oper, SImage* image) {
    switch (oper[0])
    {
    case 'f':
        flip(image);
        break;
    case 'e':
        enlarge(image);
        break;
    case 'r':
        rotate(image);
        break;
    case 's':
        shrink(image);
        break;
    default:
        break;
        // TODO: handle not implemented function
    }
}

// (width*ROW+COL)*3+1

SImage* shrink(SImage *image) {
    printf("Shrinking image \n");
    SImage *manipulated_image = handled_malloc(sizeof(SImage) * 1);
    init(manipulated_image, image->width / 2, image->height / 2);

    for (int row_id = 0; row_id < image->height / 2; ++row_id) {
        for (int pix_id = 0; pix_id < image->width / 2; ++pix_id) {
            int input_cell = (row_id * image->width / 2 + pix_id) * 3;
            int output_cell = (row_id * 2 * image->width + pix_id * 2) * 3;
            printf("in: %d out: %d\n", input_cell, output_cell);
            manipulated_image->data[input_cell] = image->data[output_cell];
            manipulated_image->data[input_cell+1] = image->data[output_cell+1];
            manipulated_image->data[input_cell+2] = image->data[output_cell+2];
        } 
    }
    *image = *manipulated_image;
    return image;
}

SImage* flip(SImage *image) {
    printf("Flipping image \n");
    SImage *manipulated_image = handled_malloc(sizeof(SImage) * 1);
    init(manipulated_image, image->width, image->height);


    size_t row_count = image->height;
    for (size_t row_id = 0; row_id < (row_count) / 2; ++row_id) {
        size_t oposite_row_id = row_count - (row_id + 1);
        memcpy(&manipulated_image->data[row_id * image->width*3],&image->data[oposite_row_id * image->width*3], image->width * 3);
        memcpy(&manipulated_image->data[oposite_row_id * image->width*3],&image->data[row_id * image->width*3], image->width * 3);
    }
    if (row_count % 2 == 1) {
        memcpy(&manipulated_image->data[(row_count/2 + 1) * image->width*3],&image->data[(row_count/2 + 1) * image->width*3], image->width*3);
    }

    *image = *manipulated_image;
    // TODO: implement freeing old image to manage memory
    return image;
}

SImage* enlarge(SImage *image) {
    printf("Enlarging image \n");
    return image;
}

SImage* rotate(SImage *image) {
    printf("Rotating image \n");
    return image;
}

void print_args(args_t* args) {
    printf("Input_file: %s \nOutput_file: %s\n Operations: ", args->in_file, args->out_file);
    for (size_t i = 0; i < args->oper_count; ++i) {
        printf("%s ", args->operations[i]);
    }
    printf("\n");
}