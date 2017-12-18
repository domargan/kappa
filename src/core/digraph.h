#ifndef KAPPA_DIGRAPH_H
#define KAPPA_DIGRAPH_H

#include <vector>
#include "vertex.h"
#include "graph.h"

typedef std::vector<dvertex> digraph_vector;

class Digraph {
private:
    unsigned int max_vertex_number; // Set a limit for the maximum possible number of vertices to fit in the graph.

    graph_vector topology; // Graph topology data + some metadata.

    unsigned int order; // Current number of vertices in the graph.
    unsigned int size; // Current number of edges in the graph.

public:
    explicit Digraph(unsigned int); // Digraph constructor.

    graph_vector get_graph_vector(); // Return the graph_vector data structure.
    void print_edges(); // (use just for testing)

    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true of the edge is in the graph.

    neighbors_vector get_in_neighborhood(unsigned int); // Return the vector of incoming neighbors of a vertex.
    neighbors_vector get_out_neighborhood(unsigned int); // Return the vector of outcoming neighbors of a vertex.
    unsigned int get_in_degree(unsigned int); // Return the in-degree of a vertex.
    unsigned int get_out_degree(unsigned int); // Return the out-degree of a vertex.
    unsigned int get_degree(unsigned int); // Return the degree of a vertex.

    unsigned int get_order(); // Return the number of vertices.
    unsigned int get_size(); // Return the number of edges.

    void increment_order(); // Increase the value for number of vertices by 1.
    void decrement_order(); // Decrease the value for number of vertices by 1.
    void increment_size(); // Increase the value for number of edges by 1.
    void decrement_size(); // Increase the value for number of edges by 1.
};

#endif //KAPPA_DIGRAPH_H
