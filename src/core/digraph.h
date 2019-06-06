#ifndef KAPPA_DIGRAPH_H
#define KAPPA_DIGRAPH_H

#include <boost/dynamic_bitset.hpp>
#include <vector>
#include <atomic>

#include "compute.h"
#include "update.h"
#include "datatypes.h"
#include "vertex.h"

// TODO: Check which get() methods should return pointers and which should return values
// TODO: Check which methods should be moved to private

// TODO: Move to dataypes.h?
typedef std::vector<Dvertex> digraph_vector_t;

// TODO: Change long to int in dynamic_bitset allocation... It does not work for some reason :)
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset_t;

typedef std::vector<state_t> state_vector_t;

typedef std::vector<vertex_id_t> vertex_queue_t;

class Digraph {
private:
    graph_size_t max_vertex_allocations; // Set a limit for the maximum possible number of vertices to fit in the graph.

    digraph_vector_t topology; // Graph topology data + some metadata.
    vertex_bitset_t vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    state_vector_t states;

    graph_size_t order; // Current number of vertices in the graph.
    graph_size_t size; // Current number of edges in the graph.

    Updating updating;
    Computation computation;

    std::atomic<timestamp_logical_t> global_logical_ts;

public:
    explicit Digraph(graph_size_t, graph_size_t, Updating, Computation); // Digraph constructor.

    digraph_vector_t get_digraph_vector(); // Return the graph_vector_t data structure.
    vertex_bitset_t get_vertex_index(); // Return index of vertices present in the graph.

    bool has_vertex(vertex_id_t); // Return true if the vertex is in the graph.
    void set_scheduled(vertex_id_t, bool);
    bool is_scheduled(vertex_id_t);
    void activate_vertex(vertex_id_t);

    void add_edge(vertex_id_t, vertex_id_t); // Add an edge between two vertices.
    void remove_edge(vertex_id_t, vertex_id_t); // Remove the edge between two vertices.
    bool has_edge(vertex_id_t, vertex_id_t); // Return true of the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector_t *get_in_neighborhood(vertex_id_t); // Return the vector of incoming neighbors of a vertex.
    neighbors_vector_t *get_out_neighborhood(vertex_id_t); // Return the vector of outcoming neighbors of a vertex.
    neighbors_vector_t get_in_out_neighborhood(vertex_id_t);
    graph_size_t get_in_degree(vertex_id_t); // Return the in-degree of a vertex.
    graph_size_t get_out_degree(vertex_id_t); // Return the out-degree of a vertex.
    graph_size_t get_degree(vertex_id_t); // Return the degree of a vertex.

    void set_state(vertex_id_t, state_t);
    state_t get_state(vertex_id_t);

    void count_order(); // Count and set the number of vertices. OBSOLETE FUNCTION
    graph_size_t get_order(); // Return the number of vertices.
    graph_size_t get_max_order(); // Return the number of pre-allocated vertex spaces.

    graph_size_t get_size(); // Return the number of edges.

    timestamp_logical_t get_incremented_global_logical_ts();
    //std::atomic<timestamp_logical_t> &get_counter();
    void set_vertex_update_ts(vertex_id_t, timestamp_logical_t);
    timestamp_logical_t get_vertex_update_ts(vertex_id_t);
    void set_vertex_on_update_ts(vertex_id_t, timestamp_logical_t);
    timestamp_logical_t get_vertex_on_update_ts(vertex_id_t);
    void set_vertex_on_activate_ts(vertex_id_t, timestamp_logical_t);
    timestamp_logical_t get_vertex_on_activate_ts(vertex_id_t);
};

#endif //KAPPA_DIGRAPH_H
