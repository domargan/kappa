#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "vertex.h"

typedef std::vector<uvertex> graph_vector;
// TODO: Change long to int in dynamic_bitset allocation... It does not work for some reason :)
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset;

class Graph {
private:
    uint32_t max_vertex_allocations; // Set a limit for the maximum possible number of vertices to fit in the graph.

    graph_vector topology; // Graph topology data + some metadata.
    vertex_bitset vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    uint32_t order; // Current number of vertices in the graph.
    uint32_t size; // Current number of edges in the graph.

public:
    explicit Graph(uint32_t, float); // Graph constructor
    graph_vector get_graph_vector(); // Return the graph_vector data structure.

    vertex_bitset get_vertex_index();
    bool has_vertex(uint32_t); // Return true if the vertex is in the graph.

    void add_edge(uint32_t, uint32_t); // Add an edge between two vertices.
    void remove_edge(uint32_t, uint32_t); // Remove the edge between two vertices.
    bool has_edge(uint32_t, uint32_t); // Return true if the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector *get_neighborhood(uint32_t); // Return the vector of neighbors of a vertex.
    uint32_t get_degree(uint32_t); // Return the degree of a vertex.

    void update_state(uint32_t, float);
    float get_state(uint32_t);
    void finalize_state(uint32_t);
    void finalize_states();

    void count_order(); // Count and set the number of vertices.
    uint32_t get_order(); // Return the number of vertices.
    uint32_t get_max_order(); // Return the number of pre-allocated vertex spaces.
    uint32_t get_size(); // Return the number of edges.

    void increment_size(); // Increase the value for number of edges by 1.
    void decrement_size(); // Increase the value for number of edges by 1.
};

#endif //KAPPA_GRAPH_H
