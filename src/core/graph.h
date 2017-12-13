#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include "vertex.h"

typedef std::vector<uvertex> graph_vector;

class Graph {
private:
    unsigned int max_vertex_number; // Set a limit for the maximum possible number of vertices to fit in the graph.
    graph_vector topology;
    unsigned int order;
    unsigned int size;
        /*
    void increment_order();
    void decrement_order();
    void increment_size();
    void decrement_size();
     */
public:
    explicit Graph(unsigned int);
    graph_vector get_graph_vector(); // Return the graph_vector data structure.
    void print_edges(); // (use just for testing)
    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true of the edge is in the graph.
    neighbors_vector get_neighborhood(unsigned int); // Return the vector of neighbors of a vertex.
    unsigned int get_degree(unsigned int); // Return the degree of a vertex.
    unsigned int get_order(); // Return the number of vertices.
    unsigned int get_size(); // Return the number of edges.
};

#endif //KAPPA_GRAPH_H
