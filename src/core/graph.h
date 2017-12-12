#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include "vertex.h"

typedef std::vector<uvertex> vertex_vector;

class Graph {
private:
    unsigned int max_vertex_number; // Set a limit for the maximum possible number of vertices to fit in the graph.
    vertex_vector topology;
public:
    explicit Graph(unsigned int);
    vertex_vector get_vertex_vector(); // Return the vertex_vector data structure.
    void print_edges(); // (use just for testing)
    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true of the edge is in the graph.
    int in_degree(unsigned int); // Return the in-degree of a vertex.
    int out_degree(unsigned int); // Return the out-degree of a vertex.
    int degree(unsigned int); // Return the degree of a vertex.
    int order(); // Return the number of vertices.
    int size(); // Return the number of edges.
};

#endif //KAPPA_GRAPH_H
