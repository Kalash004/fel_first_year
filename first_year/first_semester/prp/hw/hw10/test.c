#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct
{
    int from;
    int to;
    int cost;
} edge_t;

typedef struct
{
    edge_t *edges;
    int num_edges;
    int capacity;
} graph_t;

int main()
{
    int fd = open("./file.txt", O_RDONLY);
    struct stat sb;
    fstat(fd, &sb);
    char *file_memory = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    size_t buf_size = sb.st_size / 3; // approximation
    edge_t *buffer = handled_malloc(sizeof(edge_t) * buf_size);
    size_t char_id = 0;
    size_t used = 0;
    while (char_id < sb.st_size)
    {
        if (used == buf_size - 1)
        {
            buf_size *= 2;
            buffer = handled_realloc(buffer, sizeof(edge_t) * buf_size);
        }
        int start = get_next_int_from_line_mmap(file_memory, &char_id, sb.st_size);
        int end = get_next_int_from_line_mmap(file_memory, &char_id, sb.st_size);
        int weight = get_next_int_from_line_mmap(file_memory, &char_id, sb.st_size);

        if (start == 0 && end == 0 && weight == 0)
        {
            break;
        }

        buffer[used].from = start;
        buffer[used].to = end;
        buffer[used].cost = weight;
        ++used;
    }
    // graph->edges = buffer;
    // graph->num_edges = used;
    close(fd);
}

int get_next_int_from_line_mmap(char *file_mem, size_t *char_id, size_t last_id)
{
    char temp[11] = {0};

    char c;
    int used = 0;
    do
    {
        if (*char_id == last_id)
        {
            break;
        }

        c = file_mem[*char_id];

        if (c == ' ' || c == '\n')
        {
            break;
        }
        temp[used] = c;
        ++used;
        ++*char_id;
    } while (c != ' ' && c != '\n');
    temp[used] = '\0';
    int target = atoi(temp);
    return target;
}