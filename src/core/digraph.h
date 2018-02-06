#ifndef KAPPA_DIGRAPH_H
#define KAPPA_DIGRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "vertex.h"

// TODO: Make Digraph a child class of Graph

typedef std::vector<dvertex> digraph_vector;
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset;

class Digraph {
private:
    unsigned int max_vertex_allocations; // Set a limit for the maximum possible number of vertices to fit in the graph.

    digraph_vector topology; // Graph topology data + some metadata.
    vertex_bitset vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    unsigned int order; // Current number of vertices in the graph.
    unsigned int size; // Current number of edges in the graph.

public:
    explicit Digraph(unsigned int); // Digraph constructor.

    digraph_vector get_digraph_vector(); // Return the graph_vector data structure.
    vertex_bitset get_vertex_index(); // Return index of vertices present in the graph.

    bool has_vertex(unsigned int); // Return true if the vertex is in the graph.

    void add_edge(unsigned int, unsigned int); // Add an edge between two vertices.
    void remove_edge(unsigned int, unsigned int); // Remove the edge between two vertices.
    bool has_edge(unsigned int, unsigned int); // Return true of the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector get_in_neighborhood(unsigned int); // Return the vector of incoming neighbors of a vertex.
    neighbors_vector get_out_neighborhood(unsigned int); // Return the vector of outcoming neighbors of a vertex.
    unsigned int get_in_degree(unsigned int); // Return the in-degree of a vertex.
    unsigned int get_out_degree(unsigned int); // Return the out-degree of a vertex.
    unsigned int get_degree(unsigned int); // Return the degree of a vertex.

    void update_state(unsigned int, double);
    double get_state(unsigned int);
    void finalize_state(unsigned int);
    void finalize_states();

    void count_order(); // Count and set the number of vertices.
    unsigned int get_order(); // Return the number of vertices.
    unsigned int get_max_order(); // Return the number of pre-allocated vertex spaces.
    unsigned int get_size(); // Return the number of edges.

    void increment_size(); // Increase the value for number of edges by 1.
    void decrement_size(); // Increase the value for number of edges by 1.
};

#endif //KAPPA_DIGRAPH_H
