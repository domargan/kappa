#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "vertex.h"

typedef std::vector<Uvertex> graph_vector_t;
// TODO: Change long to int in dynamic_bitset allocation... It does not work for some reason :)
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset_t;

class Graph {
private:
    graph_size_t max_vertex_allocations; // Set a limit for the maximum possible number of vertices to fit in the graph.

    graph_vector_t topology; // Graph topology data + some metadata.
    vertex_bitset_t vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    graph_size_t order; // Current number of vertices in the graph.
    graph_size_t size; // Current number of edges in the graph.

public:
    explicit Graph(graph_size_t, state_t); // Graph constructor
    graph_vector_t get_graph_vector(); // Return the graph_vector_t data structure.

    vertex_bitset_t get_vertex_index();
    bool has_vertex(vertex_id_t); // Return true if the vertex is in the graph.

    void add_edge(vertex_id_t, vertex_id_t); // Add an edge between two vertices.
    void remove_edge(vertex_id_t, vertex_id_t); // Remove the edge between two vertices.
    bool has_edge(vertex_id_t, vertex_id_t); // Return true if the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector_t *get_neighborhood(vertex_id_t); // Return the vector of neighbors of a vertex.
    graph_size_t get_degree(vertex_id_t); // Return the degree of a vertex.

    void update_state(vertex_id_t, state_t);
    state_t get_state(vertex_id_t);
    void finalize_state(vertex_id_t);
    void finalize_states();

    void count_order(); // Count and set the number of vertices.
    graph_size_t get_order(); // Return the number of vertices.
    graph_size_t get_max_order(); // Return the number of pre-allocated vertex spaces.
    graph_size_t get_size(); // Return the number of edges.

    void increment_size(); // Increase the value for number of edges by 1.
    void decrement_size(); // Increase the value for number of edges by 1.
};

#endif //KAPPA_GRAPH_H
