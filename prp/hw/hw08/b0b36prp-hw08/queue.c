#include "queue.h"

queue_t *create_queue(int capacity)
{
    queue_t *que = malloc(sizeof(queue_t) * 1);
    void **tmp = malloc(capacity * sizeof(void *));
    que->capacity = capacity;
    que->read = 0; // set at the end of the queue
    que->write = 0;
    que->pntr = tmp;
    que->size = 0;
    return que;
}

void delete_queue(queue_t *queue)
{
    for (size_t cell_id = queue->read; cell_id < queue->write; ++cell_id)
    {
        free(queue->pntr[cell_id]);
    }
    free(queue->pntr);
    free(queue);
}

bool push_to_queue(queue_t *queue, void *data)
{
    if (queue->size == queue->capacity)
    {
        return false;
    }
    queue->pntr[queue->write] = data;
    ++queue->write;
    if ((int)queue->write > queue->capacity - 1)
        queue->write = 0;

    ++queue->size;

    // print_data_in_arr(queue);
    return true;
}

void *pop_from_queue(queue_t *queue)
{
    if (queue->size == 0)
    {
        return NULL;
    }
    void *target = queue->pntr[queue->read];
    if (target == NULL)
        return NULL;

    queue->pntr[queue->read] = NULL;
    ++queue->read;

    if ((int)queue->read > queue->capacity - 1)
        queue->read = 0;

    if (queue->size > 0)
        --queue->size;

    // print_data_in_arr(queue);
    return target;
}

void *get_from_queue(queue_t *queue, int idx)
{
    if (idx < 0)
    {
        return NULL;
    }

    if (idx > queue->capacity)
    {
        return NULL;
    }

    if (idx > queue->size - 1)
    {
        return NULL;
    }

    // TODO FINISH

    int temp = idx + (int)queue->read;

    if (temp > queue->capacity - 1)
        return NULL;
    // print_data_in_arr(queue);
    return queue->pntr[temp];
}

int get_queue_size(queue_t *queue)
{
    return queue->size;
}

void print_data_in_arr(queue_t *q)
{
    char buff[q->size + 1];
    for (int i = 0; i < q->size; ++i)
    {
        sprintf(&buff[i], "%d ", *(int *)q->pntr[i]);
    }
    buff[q->size] = '\0';
    printf("%s\n", buff);
}
