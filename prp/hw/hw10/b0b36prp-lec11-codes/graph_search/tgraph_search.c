#include "dijkstra.h"

#include <stdio.h>

#define MAX_EDGES 100 // Define a limit on the number of edges you expect in the graph

int main2(int argc, char *argv[])
{
   int ret = 0;

   if (argc < 3)
   {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   }
   else
   {
      FILE *graph_file = fopen(argv[1], "r");
      if (!graph_file)
      {
         fprintf(stderr, "Failed to open graph file %s\n", argv[1]);
         return 1;
      }

      // Read the graph data
      int edges[MAX_EDGES][3]; // Array to store edges
      int num_edges = 0;

      while (fscanf(graph_file, "%d %d %d", &edges[num_edges][0], &edges[num_edges][1], &edges[num_edges][2]) == 3)
      {
         num_edges++;
         if (num_edges >= MAX_EDGES)
         {
            fprintf(stderr, "Too many edges, increasing MAX_EDGES\n");
            break;
         }
      }
      fclose(graph_file);

      // Initialize the Dijkstra algorithm
      void *dij = dijkstra_init();

      // Set the graph using the parsed edges
      dijkstra_set_graph(num_edges, edges, dij);

      fprintf(stderr, "Find all shortest paths from node 0\n");
      dijkstra_solve(dij, 0);

      // Retrieve the solution and print it
      int solution[10][3]; // Assuming a max of 10 nodes and 3 solution details per node
      dijkstra_get_solution(dij, num_edges, solution);

      // Optionally, save or print the solution here
      fprintf(stderr, "Solution retrieved\n");
      // Print the solution (if needed)

      fprintf(stderr, "Free allocated memory\n");
      dijkstra_free(dij);
      ret = 0;
   }

   return ret;
}

int main1(int argc, char *argv[])
{
   int ret = 0;

   if (argc < 3)
   {
      fprintf(stderr, "Call as\n %s graph_file solution_file\n", argv[0]);
   }
   else
   {
      fprintf(stderr, "Load graph from %s\n", argv[1]);
      void *dij = dijkstra_init();
      dijkstra_load_graph(argv[1], dij);
      fprintf(stderr, "Find all shortest paths from the node 0\n");
      dijkstra_solve(dij, 0);
      fprintf(stderr, "Save solution to %s\n", argv[2]);
      dijkstra_save_path(dij, argv[2]);
      fprintf(stderr, "Free allocated memory\n");
      dijkstra_free(dij);
      ret = 0;
   }
   return ret;
}

/* end of tgraph_search.c */
