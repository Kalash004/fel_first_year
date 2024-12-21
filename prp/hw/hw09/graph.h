#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <stdio.h>
typedef struct
{
    int start_point;
    int end_point;
    int weight;
} edge_t;

typedef struct
{
    edge_t *edges;
    int edges_count;
} graph_t;

/* Allocate a new graph and return a reference to it. */
graph_t *allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
int get_next_int_from_line(FILE *f);
void display_graph_text(FILE *f, graph_t *graph);
void intToStr(int num, char *str, int buf_size);
void print_it(FILE *f, char *str);
/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

/* Save the graph to the text file. */
void save_txt(const graph_t *const graph, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const graph_t *const graph, const char *fname);

#endif // __GRAPH_H__