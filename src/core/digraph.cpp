#include <iostream>
#include <algorithm>

#include "digraph.h"

// TODO: Implement vertex iterator (instead of iterating until vertex_index.size())
// TODO: Do something smarter about getting size; do not call vertex_index.size() all the time
// TODO: For every iteration of vertices from 0 to vertex_index.size() check if the vertex exists before any operations
// TODO: Remove all calls to vertex_index.size() and replace them with a variable

Digraph::Digraph(uint32_t v_num, state_t init_state) {
    std::cout << "Digraph constructor called.\n" << std::endl;

    max_vertex_allocations = v_num + 1;

    vertex_index = boost::dynamic_bitset<>(max_vertex_allocations);

    for (int i = 0; i < max_vertex_allocations; i++) {
        Dvertex dv{};
        dv.in_neighbors = new neighbors_vector_t;
        dv.out_neighbors = new neighbors_vector_t;
        dv.in_degree = 0;
        dv.out_degree = 0;
        dv.state = init_state;
        dv.state_temp = init_state;

        topology.push_back(dv);
    }

    state_change_monitor = false;
    state_change_tolerance = 0.0;

    order = 0;
    size = 0;

    std::cout << "Digraph structure initialized with " << v_num << " vertex entries.\n" << std::endl;
}

digraph_vector_t Digraph::get_digraph_vector() {
    return topology;
}

vertex_bitset_t Digraph::get_vertex_index() {
    return vertex_index;
}

void Digraph::print_edges() {
    for (std::vector<Dvertex>::size_type v = 0; v != topology.size(); v++) {
        std::cout << "v" << v << ": " << std::endl;
        for (uint32_t in_neighbor : *topology[v].in_neighbors) {
            std::cout << in_neighbor << " " << std::endl;
        }
        for (uint32_t out_neighbor : *topology[v].out_neighbors) {
            std::cout << out_neighbor << " " << std::endl;
        }
        std::cout << std::endl;
    std::cout << std::endl;
    }
}

bool Digraph::has_vertex(uint32_t v) {
    return vertex_index[v];
}

bool Digraph::has_edge(uint32_t src_v, uint32_t dst_v) {
    // std::find() has a worst-case time of O(n) in the distance between first and last.

    // TODO: Remove duplicated neighbors vectors and implement a new way of checking for existance of the
    // edges that are in the opposite direction of those represented by neighbor vectors
    return std::find(topology[src_v].out_neighbors->begin(), topology[src_v].out_neighbors->end(), dst_v)
           != topology[src_v].out_neighbors->end();
}

void Digraph::add_edge(uint32_t src_v, uint32_t dst_v) {
    if (!has_edge(src_v, dst_v)) {
        // std::vector.push_back() has a O(1) amortized time.
        // Reallocation may happen.
        // If a reallocation happens, the reallocation is O(n).

        // TODO: Remove duplicated neighbors vectors, store just one type of neighbors
        topology[src_v].out_neighbors->push_back(dst_v);
        topology[dst_v].in_neighbors->push_back(src_v);

        topology[src_v].out_degree++;
        topology[dst_v].in_degree++;

        vertex_index[src_v] = 1;
        vertex_index[dst_v] = 1;

        increment_size();
    }
}

void Digraph::remove_edge(uint32_t src_v, uint32_t dst_v) {
    if (has_edge(src_v, dst_v)) {
        // Erase-remove has a worst-case time of O(n).

        // TODO: Remove duplicated neighbors vectors
        topology[src_v].out_neighbors->erase
                (std::remove(topology[src_v].out_neighbors->begin(), topology[src_v].out_neighbors->end(), dst_v),
                 topology[src_v].out_neighbors->end());

        topology[dst_v].in_neighbors->erase
                (std::remove(topology[dst_v].in_neighbors->begin(), topology[dst_v].in_neighbors->end(), src_v),
                 topology[dst_v].in_neighbors->end());

        topology[src_v].out_degree--;
        topology[dst_v].in_degree--;

        decrement_size();

        if(get_out_degree(src_v) == 0 && get_in_degree(src_v) == 0) {
            vertex_index[src_v] = 0;
        }

        if(get_out_degree(dst_v) == 0 && get_in_degree(dst_v) == 0) {
            vertex_index[dst_v] = 0;
        }
    }
}

neighbors_vector_t *Digraph::get_in_neighborhood(uint32_t v) {
    // TODO: Remove duplicated neighbors vectors
    return topology[v].in_neighbors;
}

neighbors_vector_t *Digraph::get_out_neighborhood(uint32_t v) {
    return topology[v].out_neighbors;
}

uint32_t Digraph::get_in_degree(uint32_t v) {
    // TODO: Finish the and test the implementation of degree counting
    return topology[v].in_degree;
    //return static_cast<uint32_t>(get_in_neighborhood(v)->size());
}

uint32_t Digraph::get_out_degree(uint32_t v) {
    // TODO: Finish the and test the implementation of degree counting
    return topology[v].out_degree;
    //return static_cast<uint32_t>(get_out_neighborhood(v)->size());
}

uint32_t Digraph::get_degree(uint32_t v) {
    return get_in_degree(v) + get_out_degree(v);
}

void Digraph::update_state(uint32_t v, state_t state_new) {
    topology[v].state_temp = state_new;

    if(std::abs(state_new - topology[v].state) >= state_change_tolerance){
        state_change_monitor = true;
    }
}

state_t Digraph::get_state(uint32_t v) {
    return topology[v].state;
}

void Digraph::finalize_state(uint32_t v) {
    if(has_vertex(v)){
        topology[v].state = topology[v].state_temp;
    }
}

void Digraph::finalize_states() {
    // TODO: Each state field in the vertex struct should be a pointer to a field in an external vector that contains
    // the states of all vertices (there are two vectors, one for current states and one for previous states)
    // Finalize states should be just a swap() function over those two vectors.

    uint32_t max_order = get_max_order();

    // For each v in the graph exchange state
    for(uint32_t i=0; i<max_order; i++) {
        finalize_state(i);
    }
}

void Digraph::set_state_change_tolerance(state_t epsilon){
    state_change_tolerance = epsilon;
}

void Digraph::count_order() {
    order = static_cast<uint32_t>(vertex_index.count());
}

uint32_t Digraph::get_order() {
    return order;
}

uint32_t Digraph::get_max_order() {
    return max_vertex_allocations;
}

uint32_t Digraph::get_size() {
    return size;
}

void Digraph::increment_size() {
    size++;
}

void Digraph::decrement_size() {
    size--;
}