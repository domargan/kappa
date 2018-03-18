#ifndef KAPPA_DIGRAPH_H
#define KAPPA_DIGRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>
#include <boost/circular_buffer.hpp>

#include "vertex.h"

// TODO: Make Digraph a child class of Graph

// TODO: Check which get() methods should return pointers and which should return values

// TODO: Check which methods should be moved to private

typedef std::vector<Dvertex> digraph_vector_t;
// TODO: Change long to int in dynamic_bitset allocation... It does not work for some reason :)
typedef boost::dynamic_bitset<unsigned long, std::allocator<unsigned long>> vertex_bitset_t;
typedef boost::circular_buffer<uint32_t> vertex_queue_t;

class Digraph {
private:
    uint32_t max_vertex_allocations; // Set a limit for the maximum possible number of vertices to fit in the graph.

    digraph_vector_t topology; // Graph topology data + some metadata.
    vertex_bitset_t vertex_index; // 1 if the vertex is present, 0 if the vertex is not in the graph.

    // TODO: Maybe visited_verts should be a part of vertex struct?
    vertex_bitset_t visited_verts; // 1 if the vertex has been visited by an algorithm, 0 if not.

    vertex_queue_t touched_src_verts; // A list of source vertices touched by the most recent batch of updates.

    uint32_t order; // Current number of vertices in the graph.
    uint32_t size; // Current number of edges in the graph.

public:
    explicit Digraph(uint32_t, state_t, uint32_t); // Digraph constructor.

    digraph_vector_t get_digraph_vector(); // Return the graph_vector_t data structure.
    vertex_bitset_t get_vertex_index(); // Return index of vertices present in the graph.

    bool has_vertex(uint32_t); // Return true if the vertex is in the graph.

    void add_edge(uint32_t, uint32_t); // Add an edge between two vertices.
    void remove_edge(uint32_t, uint32_t); // Remove the edge between two vertices.
    bool has_edge(uint32_t, uint32_t); // Return true of the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector_t *get_in_neighborhood(uint32_t); // Return the vector of incoming neighbors of a vertex.
    neighbors_vector_t *get_out_neighborhood(uint32_t); // Return the vector of outcoming neighbors of a vertex.
    uint32_t get_in_degree(uint32_t); // Return the in-degree of a vertex.
    uint32_t get_out_degree(uint32_t); // Return the out-degree of a vertex.
    uint32_t get_degree(uint32_t); // Return the degree of a vertex.

    void update_state(uint32_t, state_t);
    state_t get_state(uint32_t);
    void finalize_state(uint32_t);
    void finalize_states();
    bool state_change_monitor;
    state_t state_change_tolerance;
    void set_state_change_tolerance(state_t);

    vertex_queue_t *get_touched_src_verts();

    vertex_bitset_t *get_visited_verts();
    void set_visited(uint32_t);
    void unset_visited(uint32_t);
    void reset_visited_verts();

    void count_order(); // Count and set the number of vertices. OBSOLETE FUNCTION
    uint32_t get_order(); // Return the number of vertices.
    uint32_t get_max_order(); // Return the number of pre-allocated vertex spaces.
    // TODO: Remove increment and decrement functions.. Maybe we don't need them... (Just do order++ or order-- manually...)
    void increment_order(); // Increase the value of number of vertices by 1.
    void decrement_order(); // Decrease the value of number of vertices by 1.

    uint32_t get_size(); // Return the number of edges.
    // TODO: Remove increment and decrement functions.. Maybe we don't need them... (Just do size++ or size-- manually...)
    void increment_size(); // Increase the value of number of edges by 1.
    void decrement_size(); // Decrease the value of number of edges by 1.
};

#endif //KAPPA_DIGRAPH_H
