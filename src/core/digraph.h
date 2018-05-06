#ifndef KAPPA_DIGRAPH_H
#define KAPPA_DIGRAPH_H

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "datatypes.h"
#include "vertex.h"


// TODO: Make Digraph a child class of Graph

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
    state_vector_t states_temp;

    bool state_change_monitor;
    state_t state_change_tolerance;

    // TODO: Maybe visited_verts should be a part of vertex struct?
    vertex_bitset_t visited_verts; // 1 if the vertex has been visited by an algorithm, 0 if not.

    vertex_queue_t touched_src_verts; // A list of source vertices touched by the most recent batch of updates.

    graph_size_t order; // Current number of vertices in the graph.
    graph_size_t size; // Current number of edges in the graph.

public:
    explicit Digraph(graph_size_t, state_t, graph_size_t); // Digraph constructor.

    digraph_vector_t get_digraph_vector(); // Return the graph_vector_t data structure.
    vertex_bitset_t get_vertex_index(); // Return index of vertices present in the graph.

    bool has_vertex(vertex_id_t); // Return true if the vertex is in the graph.

    void add_edge(vertex_id_t, vertex_id_t); // Add an edge between two vertices.
    void add_edge_populate(vertex_id_t, vertex_id_t); // JUST FOR TESTING
    void remove_edge(vertex_id_t, vertex_id_t); // Remove the edge between two vertices.
    bool has_edge(vertex_id_t, vertex_id_t); // Return true of the edge is in the graph.

    void print_edges(); // (use just for testing)

    neighbors_vector_t *get_in_neighborhood(vertex_id_t); // Return the vector of incoming neighbors of a vertex.
    neighbors_vector_t *get_out_neighborhood(vertex_id_t); // Return the vector of outcoming neighbors of a vertex.
    graph_size_t get_in_degree(vertex_id_t); // Return the in-degree of a vertex.
    graph_size_t get_out_degree(vertex_id_t); // Return the out-degree of a vertex.
    graph_size_t get_degree(vertex_id_t); // Return the degree of a vertex.

    void update_state(vertex_id_t, state_t);
    state_t get_state(vertex_id_t);
    void finalize_state(vertex_id_t);
    void finalize_states();
    void set_state_change_tolerance(state_t);
    bool state_changed();
    void set_state_change();
    void unset_state_change();

    vertex_queue_t *get_touched_src_verts();
    void reset_touched_src_verts();

    vertex_bitset_t *get_visited_verts();
    void set_visited(vertex_id_t);
    void unset_visited(vertex_id_t);
    void reset_visited_verts();
    bool has_been_visited(vertex_id_t);

    void count_order(); // Count and set the number of vertices. OBSOLETE FUNCTION
    graph_size_t get_order(); // Return the number of vertices.
    graph_size_t get_max_order(); // Return the number of pre-allocated vertex spaces.
    // TODO: Remove increment and decrement functions.. Maybe we don't need them... (Just do order++ or order-- manually...)
    void increment_order(); // Increase the value of number of vertices by 1.
    void decrement_order(); // Decrease the value of number of vertices by 1.

    graph_size_t get_size(); // Return the number of edges.
    // TODO: Remove increment and decrement functions.. Maybe we don't need them... (Just do size++ or size-- manually...)
    void increment_size(); // Increase the value of number of edges by 1.
    void decrement_size(); // Decrease the value of number of edges by 1.
};

#endif //KAPPA_DIGRAPH_H
