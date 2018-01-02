#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "vertex.h"

typedef std::vector<uvertex> graph_vector;
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset;

class Graph {
private:
    unsigned int max_vertex_number; // Set a limit for the maximum possible number of vertices to fit in the graph.

    graph_vector topology; // Graph topology data + some metadata.
    vertex_bitset vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    unsigned int order; // Current number of vertices in the graph.
    unsigned int size; // Current number of edges in the graph.

public:
    explicit Graph(unsigned int); // Graph constructor
    graph_vector get_graph_vector(); // Return the graph_vector data structure.

    vertex_bitset get_vertex_index();
    bool has_vertex(unsigned int); // Return true if the vertex is in the graph.

    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true if the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector get_neighborhood(unsigned int); // Return the vector of neighbors of a vertex.
    unsigned int get_degree(unsigned int); // Return the degree of a vertex.

    void update_state(unsigned int, float);
    float get_state(unsigned int);
    void finalize_states();

    unsigned int get_order(); // Return the number of vertices.
    unsigned int get_size(); // Return the number of edges.

    void increment_size(); // Increase the value for number of edges by 1.
    void decrement_size(); // Increase the value for number of edges by 1.
};

#endif //KAPPA_GRAPH_H
