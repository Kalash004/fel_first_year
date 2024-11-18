#include <stdio.h>
#include <stdlib.h>

#define RET_ERROR_INIT 100
#define RET_ERROR_EMPTY 101
#define RET_ERROR_CHECK 102
#define RET_ERROR_FREE 103

#ifndef INIT_SIZE
#define INIT_SIZE 10
#endif

// FIFO
typedef struct Queue
{
  int *list;
  size_t size;
  size_t first;
  size_t last;
} Queue;

/**
 * @brief Initialize queue with given size.
 *
 * @param init_size
 * @return Queue* Empty queue or NULL if error occurs
 **/
Queue *init_queue(size_t init_size);

/**
 * @brief Checks if queue is empty.
 *
 * @param q Queue
 * @return int Return 1 if q is empty, otherwise 0
 */
int is_empty(Queue *q);

/**
 * @brief Push an item to end of the queue.
 *  Check if queue is full or not.
 *
 * @param q Queue
 * @param item Item to be added to queue
 */
void push(Queue *q, int item);

/**
 * @brief Remove first inserted item from queue and return it.
 *
 * @param q Queue
 * @return int First inserted item
 */
int pop(Queue *q);

/**
 * @brief Return first inserted item from queue, DO NOT REMOVE.
 *
 * @param q Queue
 * @return int First inserted item
 */
int front(Queue *q);

/**
 * @brief Return last inserted item from queue, DO NOT REMOVE.
 *
 * @param q Queue
 * @return int Last inserted item
 */
int last(Queue *q);

/**
 * @brief Prints queue
 *
 * @param q Queue
 */
void print(Queue *q);

/**
 * @brief Frees all allocated memory
 *
 * @param q
 */
void free_queue(Queue *q);

int main(int argc, char const *argv[])
{
  Queue *q = init_queue(INIT_SIZE);
  if (!q)
  {
    fprintf(stderr, "Error occured while initializing the queue.\n");
    return RET_ERROR_INIT;
  }

  int ret = is_empty(q);
  if (ret == 0)
  {
    fprintf(stderr, "Error occured while checking the queue.\n");
    return RET_ERROR_EMPTY;
  }

  int first = 10;
  printf("push: %d\n", first);
  push(q, first);
  for (int i = 1; i < 3; i++) {
    printf("push: %d\n", i);
    push(q, i);
  }

  ret = front(q);
  if (ret != first) {
    fprintf(stderr, "Error occured, the first element do not match: %d != %d.\n", ret, first);
    return RET_ERROR_CHECK;
  }

  print(q);
  ret = pop(q);
  printf("pop: %d\n", ret);

  while (!is_empty(q)) {
    ret = pop(q);
    printf("pop: %d\n", ret);
  }

  free_queue(q);

  if(!q) {
    fprintf(stderr, "Error occured while freeing queue.\n");
    return RET_ERROR_FREE;
  }

  return 0;
}