#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "dijkstra.h"
#include "graph.h"
#include "my_malloc.h"
#include "pq_heap.h"
#include "load_simple.h"
#include <string.h>

typedef struct
{
   int edge_start; // index to the first edge in the array of edges (-1 if does not exist)
   int edge_count; // number of edges (may be 0)
   int parent;     // index to the parent node on the shortest path from the given node
   int cost;       // cost of the shortest path from the starting node to this node
} node_t;

typedef struct
{
   graph_t *graph;
   node_t *nodes;
   int num_nodes;  // number of nodes;
   int start_node; //
} dijkstra_t;

// - function ----------------------------------------------------------------
void *dijkstra_init(void)
{
   dijkstra_t *dij = myMalloc(sizeof(dijkstra_t));
   dij->nodes = NULL;
   dij->num_nodes = 0;
   dij->start_node = -1;
   dij->graph = allocate_graph();
   if (!dij->graph)
   {
      free(dij);
      dij = NULL;
   }
   return dij;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_load_graph(const char *filename, void *dijkstra)
{
   _Bool ret = false;
   dijkstra_t *dij = (dijkstra_t *)dijkstra;
   if (
       dij && dij->graph)
   {
      load_txt(filename, (graph_t *)dij->graph);
      int m = -1;
      for (int i = 0; i < dij->graph->num_edges; ++i)
      {
         const edge_t *const e = &(dij->graph->edges[i]); // use pointer to avoid copying
         m = m < e->from ? e->from : m;
         m = m < e->to ? e->to : m;
      }
      m += 1; // m is the index therefore we need +1 for label 0
      dij->nodes = myMalloc(sizeof(node_t) * m);

      dij->num_nodes = m;

      // 2nd initialization of the nodes
      for (int i = 0; i < m; ++i)
      {
         dij->nodes[i].edge_start = -1;
         dij->nodes[i].edge_count = 0;
         dij->nodes[i].parent = -1;
         dij->nodes[i].cost = -1;
      }

      // 3nd add edges to the nodes
      for (int i = 0; i < dij->graph->num_edges; ++i)
      {
         int cur = dij->graph->edges[i].from;
         if (dij->nodes[cur].edge_start == -1)
         {                                  // first edge
            dij->nodes[cur].edge_start = i; // mark the first edge in the array of edges
         }
         dij->nodes[cur].edge_count += 1; // increase number of edges
      }
      ret = true;
   }
   return ret;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_solve(void *dijkstra, int label)
{
   dijkstra_t *dijstrik = (dijkstra_t *)dijkstra;
   // Gates
   if (!dijstrik)
   {
      return false;
   }
   if (label < 0)
   {
      return false;
   }
   if (label >= dijstrik->num_nodes)
   {
      return false;
   }
   // Gates end

   dijstrik->start_node = label;

   void *pq = pq_alloc(dijstrik->num_nodes);

   dijstrik->nodes[label].cost = 0; // initialize the starting node
   pq_push(pq, label, 0);

   int cur_label;
   while (!pq_is_empty(pq) && pq_pop(pq, &cur_label))
   {
      node_t *cur = &(dijstrik->nodes[cur_label]);
      for (int i = 0; i < cur->edge_count; ++i)
      {                                                                 // relax all children
         edge_t *edge = &(dijstrik->graph->edges[cur->edge_start + i]); // avoid copying
         node_t *to = &(dijstrik->nodes[edge->to]);
         const int cost = cur->cost + edge->cost;
         if (to->cost == -1)
         { // the node to has not been visited yet
            to->cost = cost;
            to->parent = cur_label;
            pq_push(pq, edge->to, cost);
         }
         else if (cost < to->cost)
         { // already relaxed check if we can make a shortcut to child node via the current node
            to->cost = cost;
            to->parent = cur_label;
            pq_update(pq, edge->to, cost);
         }
      } // end all children of the cur node;
   } // end pq_is_empty

   pq_free(pq); // release the memory

   return true;
}

// - function ----------------------------------------------------------------
_Bool dijkstra_save_path(const void *dijkstra, const char *filename)
{
   _Bool ret = false;
   const dijkstra_t *const dij = (dijkstra_t *)dijkstra;
   if (!dij)
   {
      return false;
   }
   FILE *f = fopen(filename, "w");
   if (!f)
   {
      return false;
   }
   size_t buf_size = 1000000;
   size_t cursor = 0;
   char *buffer = myMalloc(buf_size * sizeof(char));
   for (int i = 0; i < dij->num_nodes; ++i)
   {
      if (buf_size - cursor < 100)
      {
         buf_size += 100000;
         char *temp = realloc(buffer, buf_size);
         if (!temp)
         {
            printf("Reallocation error in dijkstra save");
            free(buffer);
            exit(-1);
         }
         buffer = temp;
      }

      int shift = 0;
      int dig_count1 = intToStr_dijk(i, buffer, 10, cursor + shift);
      shift += dig_count1;
      buffer[cursor + shift] = ' ';
      shift += 1;

      int end = dij->nodes[i].cost;
      int dig_count2 = intToStr_dijk(end, buffer, 10, cursor + shift);
      shift += dig_count2;
      buffer[cursor + shift] = ' ';
      shift += 1;

      int weight = dij->nodes[i].parent;
      int dig_count3 = intToStr_dijk(weight, buffer, 10, cursor + shift);
      shift += dig_count3;
      buffer[cursor + shift] = '\n';
      shift += 1;

      cursor += shift;
   }
   buffer[cursor] = '\0';
   fwrite(buffer, sizeof(char), cursor + 1, f);
   ret = fclose(f) == 0;
   return ret;
}

int intToStr_dijk(int num, char *str, int buf_size, int start)
{
   int sign_field = 0, isNegative = 0;

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
      str[start] = '-';
      sign_field++;
   }

   // Build the string in the correct order
   for (int j = digits - 1; j >= 0; --j)
   {
      str[sign_field + j + start] = (num % 10) + '0';
      num /= 10;
   }

   // Null-terminate the string
   if (isNegative)
      ++digits;

   return digits;
}

// - function ----------------------------------------------------------------
void dijkstra_free(void *dijkstra)
{
   dijkstra_t *dij = (dijkstra_t *)dijkstra;
   if (dij)
   {
      if (dij->graph)
      {
         free_graph(&(dij->graph));
      }

      if (dij->nodes)
      {
         free(dij->nodes);
      }

      free(dij);
   }
}

_Bool dijkstra_get_solution(const void *dijkstra, int n, int solution[][3])
{
   if (!dijkstra)
   {
      return false;
   }
   dijkstra_t *dijk = (dijkstra_t *)dijkstra;
   if (n != dijk->num_nodes)
   {
      return false;
   }
   if (!solution)
   {
      return false;
   }

   for (int i = 0; i < n; ++i)
   {
      solution[i][0] = i;
      solution[i][1] = dijk->nodes[i].cost;
      solution[i][2] = dijk->nodes[i].parent;
   }
   return true;
}

/*
 * Set the graph to the dijkstra structure instead of direct
 * loading the file as in dijkstra_load_graph() function.
 * The given array edges should not be directly used for the
 * internal representation of the graph in the dijkstra algorithm
 *
 * e - number of edges, i.e., size of the array
 *
 * return: true on success; false on an error, e.g., memory allocation
 */

// Uncomment for the HW 10
_Bool dijkstra_set_graph(int e, int edges[][3], void *dijkstra)
{
   dijkstra_t *dij = (dijkstra_t *)dijkstra;
   if (!dij || !dij->graph || e <= 0 || !edges)
   {
      return false;
   }

   dij->graph->edges = myMalloc(sizeof(edge_t) * e);

   dij->graph->num_edges = e;
   int max_node = -1;
   for (int i = 0; i < e; ++i)
   {
      dij->graph->edges[i].from = edges[i][0];
      dij->graph->edges[i].to = edges[i][1];
      dij->graph->edges[i].cost = edges[i][2];

      if (edges[i][0] > max_node)
      {
         max_node = edges[i][0];
      }
      if (edges[i][1] > max_node)
      {
         max_node = edges[i][1];
      }
   }

   dij->num_nodes = max_node + 1;
   dij->nodes = myMalloc(sizeof(node_t) * dij->num_nodes);
   if (!dij->nodes)
   {
      free(dij->graph->edges);
      dij->graph->edges = NULL;
      return false;
   }

   for (int i = 0; i < dij->num_nodes; ++i)
   {
      dij->nodes[i].edge_start = -1;
      dij->nodes[i].edge_count = 0;
      dij->nodes[i].parent = -1;
      dij->nodes[i].cost = -1;
   }

   for (int i = 0; i < e; ++i)
   {
      int from = dij->graph->edges[i].from;
      if (dij->nodes[from].edge_start == -1)
      {
         dij->nodes[from].edge_start = i;
      }
      dij->nodes[from].edge_count++;
   }

   return true;
}
/* end of dijkstra.c */
