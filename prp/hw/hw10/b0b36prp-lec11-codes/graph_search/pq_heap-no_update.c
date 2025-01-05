/*
 * File name: pq_heap-no_update.c
 * Date:      2016/08/06 21:03
 * Author:    Jan Faigl
 */

#include "pq_heap.h"
#include "my_malloc.h"

#include <stdlib.h>
#include <stdbool.h>

/*
 * Simple implementation of the prioriry queue for the
 * dijkstra algorithm based on array and with log(n)
 * complexity of the pop function
 */
typedef struct
{
   int size;     // the maximal number of entries in the array
   int len;      // the current number of entries in the array
   int *cost;    // array with entries (costs)
   int *label;   // array with vertex labels
   int *heapIDX; // this is need for implementing pq_update()
                 // array with indexes of cost vertices, i.e.,where the particular
                 // heapIDX[id] is the index where the cost of the vertex with the
                 // label id is tored in cost.
                 // E.g., cost of the vertex with the label id is at the heapIDX[id]
                 // position in the cost array, thus, cost is cost[ heapIDX[ id ] ]
} pq_heap_s;

#define GET_PARENT(i) ((i - 1) >> 1)
#define GET_LEFT(i) ((i << 1) + 1)

// - local function for implementing priority queue as heap ------------------
static void pq_down(pq_heap_s *pq);
static void pq_swap(pq_heap_s *pq, int i, int j);

// - function ----------------------------------------------------------------
void *pq_alloc(int size)
{
   pq_heap_s *pq = myMalloc(sizeof(pq_heap_s));
   pq->size = size;
   pq->len = 0;
   pq->cost = myMalloc(sizeof(int) * size);
   pq->label = myMalloc(sizeof(int) * size);
   pq->heapIDX = myMalloc(sizeof(int) * size);
   if (pq->cost && pq->label && pq->heapIDX)
   { // allocation fails
      for (int i = 0; i < pq->size; ++i)
      {
         pq->heapIDX[i] = -1; // set -1 to indicate that currently non of the graph vertices are placed in the heap
      }
   }
   else
   { // cleanup if something goes wrong
      if (pq->cost)
      {
         free(pq->cost);
      }
      if (pq->label)
      {
         free(pq->label);
      }
      if (pq->heapIDX)
      {
         free(pq->heapIDX);
      }
      free(pq);
      pq = NULL;
   }
   return pq;
}

// - function ----------------------------------------------------------------
void pq_free(void *_pq)
{
   pq_heap_s *pq = (pq_heap_s *)_pq;
   if (pq)
   {
      free(pq->cost);
      free(pq->label);
      free(pq->heapIDX);
      free(pq);
   }
}

// - function ----------------------------------------------------------------
_Bool pq_is_empty(const void *_pq)
{
   pq_heap_s *pq = (pq_heap_s *)_pq;
   return pq && pq->len == 0 ? true : false;
}

// - function ----------------------------------------------------------------
_Bool pq_push(void *_pq, int label, int cost)
{
   _Bool ret = false;
   pq_heap_s *pq = (pq_heap_s *)_pq;
   if (pq && pq->len < pq->size && label >= 0 && label < pq->size)
   {
      pq->cost[pq->len] = cost;     // add the cost to the next slot in cost
      pq->label[pq->len] = label;   // add vertex label
      pq->heapIDX[label] = pq->len; // remember position of the label cost

      int cur = pq->len; // index of the entry added to the heap
      int parent = GET_PARENT(cur);
      while (cur >= 1 && pq->cost[parent] > pq->cost[cur])
      {
         pq_swap(pq, parent, cur);
         cur = parent;
         parent = GET_PARENT(cur);
      }
      pq->len += 1;
      ret = true;
   }
   return ret;
}

// Check this
// - function ----------------------------------------------------------------
_Bool pq_update_old(void *_pq, int label, int cost)
{
   pq_heap_s *pq = (pq_heap_s *)_pq;
   if (!pq)
      return false;

   if (pq->len >= pq->size)
      return false;

   if (label < 0)
      return false;

   if (label >= pq->size)
      return false;

   if (pq->heapIDX[label] == -1) // vertex with the label is in the pq
      return false;

   pq->cost[pq->heapIDX[label]] = cost; // update the cost, but heap property is not satified
   // assert(pq_is_heap(pq, 0));

   pq_heap_s *pqBackup = (pq_heap_s *)pq_alloc(pq->size); // create backup of the heap
   pqBackup->len = pq->len;
   for (int i = 0; i < pq->len; ++i)
   {                                   // backup the heep
      pqBackup->cost[i] = pq->cost[i]; // just cost and labels
      pqBackup->label[i] = pq->label[i];
   }

   pq->len = 0; // clear all vertices in the current heap
   for (int i = 0; i < pqBackup->len; ++i)
   { // create new heap from the backup with updated cost for index
      pq_push(pq, pqBackup->label[i], pqBackup->cost[i]);
   }
   pq_free(pqBackup); // release the queue
   // assert(pq_is_heap(pq, 0));
   return true;
}

_Bool pq_update(void *_pq, int label, int cost)
{
   pq_heap_s *pq = (pq_heap_s *)_pq;
   if (!pq)
      return false;

   if (pq->len >= pq->size)
      return false;

   if (label < 0)
      return false;

   if (label >= pq->size)
      return false;

   if (pq->heapIDX[label] == -1) // vertex with the label is in the pq
      return false;

   int idx = pq->heapIDX[label];
   pq->cost[idx] = cost; // update the cost, but heap property is not satified
   // assert(pq_is_heap(pq, 0));
   while (cost < pq->cost[GET_PARENT(idx)])
   {
      pq_swap(pq, idx, GET_PARENT(idx));
      idx = pq->heapIDX[label];
   }
   while (cost)
   {
      /* code */
   }
   

   return true;
}

// - function ----------------------------------------------------------------
_Bool pq_pop(void *_pq, int *oLabel)
{
   _Bool ret = false;
   pq_heap_s *pq = (pq_heap_s *)_pq;
   if (pq && pq->len > 0)
   {
      *oLabel = pq->label[0];
      pq->heapIDX[*oLabel] = -1; // mark the vertex is not in the heap
      pq->len -= 1;
      pq->label[0] = pq->label[pq->len];
      pq->cost[0] = pq->cost[pq->len];
      pq->heapIDX[pq->label[0]] = 0;
      pq_down(pq);
      ret = true;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool pq_is_heap(void *heap, int n)
{
   _Bool ret = true;
   pq_heap_s *pq = (pq_heap_s *)heap;
   int l = 2 * n + 1;
   int r = l + 1;

   if (l < pq->len)
   {
      ret = (pq->cost[l] < pq->cost[n]) ? false : pq_is_heap(heap, l);
   }
   if (r < pq->len)
   {
      ret = ret && ((pq->cost[r] < pq->cost[n]) ? false : pq_is_heap(heap, r));
   }
   return ret;
}

// - local function ----------------------------------------------------------
static void pq_down(pq_heap_s *pq)
{
   int current;
   int left_child, right_child;
   int best;

   current = 0;
   left_child = GET_LEFT(current);
   while (left_child < pq->len)
   {
      right_child = left_child + 1;
      if (pq->cost[current] > pq->cost[left_child])
      {
         best = left_child; // left is the candite
      }
      else
      {
         best = current;
      }
      if (right_child < pq->len && pq->cost[best] > pq->cost[right_child])
      {
         best = right_child; // right is the choice
      }
      if (best != current)
      { // lower value found
         pq_swap(pq, current, best);
         current = best;
         left_child = GET_LEFT(current);
      }
      else
      {
         break;
      }
   }
   // check_heap(0, heap, nodes);
}

// - local function ----------------------------------------------------------
static void pq_swap(pq_heap_s *pq, int i, int j)
{
   int t;
   pq->heapIDX[pq->label[i]] = j;
   pq->heapIDX[pq->label[j]] = i;
   t = pq->label[i];
   pq->label[i] = pq->label[j];
   pq->label[j] = t;
   t = pq->cost[i];
   pq->cost[i] = pq->cost[j];
   pq->cost[j] = t;
}

/* end of pq_heap-no_update.c */
