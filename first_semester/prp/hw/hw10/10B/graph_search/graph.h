#ifndef __GRAPH_H__
#define __GRAPH_H__

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

/* Allocate a new graph and return a reference to it. */
graph_t *allocate_graph();
/* Free all allocated memory and set reference to the graph to NULL. */
void free_graph(graph_t **graph);

/* Load a graph from the text file. */
void load_txt(const char *fname, graph_t *graph);
int get_next_int_from_line(FILE *f);
void save_graph_text(FILE *f, const graph_t *const graph);
void intToStr(int num, char *str, int buf_size);
void print_it(FILE *f, char *str);
/* Load a graph from the binary file. */
void load_bin(const char *fname, graph_t *graph);

int read_int(FILE *f);

/* Save the graph to the text file. */
void save_txt(const graph_t *const graph, const char *fname);
/* Save the graph to the binary file. */
void save_bin(const graph_t *const graph, const char *fname);

void write_int(int source, FILE *file);

graph_t *enlarge_graph(graph_t *g);

void print_graph(graph_t *g);

int get_next_int_from_line_mmap(char *file_mem, size_t *char_id, size_t last_id);

int fast_atoi(const char *str);
#endif
