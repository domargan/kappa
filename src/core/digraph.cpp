#include <iostream>
#include <algorithm>

#include "digraph.h"

// TODO: Implement vertex iterator (instead of iterating until vertex_index.size())
// TODO: Do something smarter about getting size; do not call vertex_index.size() all the time
// TODO: For every iteration of vertices from 0 to vertex_index.size() check if the vertex exists before any operations
// TODO: Remove all calls to vertex_index.size() and replace them with a variable

Digraph::Digraph(graph_size_t v_num, state_t init_state, graph_size_t update_batch_size) {
    std::cout << "Digraph constructor called.\n" << std::endl;

    max_vertex_allocations = v_num + 1;

    vertex_index = boost::dynamic_bitset<>(max_vertex_allocations);

    states.reserve(max_vertex_allocations);
    states_temp.reserve(max_vertex_allocations);

    std::fill(states.begin(),states.end(), init_state);
    std::fill(states_temp.begin(),states_temp.end(), init_state);

    visited_verts = boost::dynamic_bitset<>(max_vertex_allocations);

    for (int i = 0; i < max_vertex_allocations; i++) {
        Dvertex dv{};
        dv.in_neighbors = new neighbors_vector_t;
        dv.out_neighbors = new neighbors_vector_t;
        dv.in_degree = 0;
        dv.out_degree = 0;
        dv.state = &states[i];
        dv.state_temp = &states_temp[i];

        topology.push_back(dv);
    }

    state_change_monitor = false;
    state_change_tolerance = 0.0;

    //touched_src_verts = boost::circular_buffer<vertex_id_t>(update_batch_size); // The number of touched source vertices can at maximum be equal to batch size
    touched_src_verts.reserve(update_batch_size); // The number of touched source vertices can at maximum be equal to batch size

    order = 0;
    size = 0;

    std::cout << "Digraph structure initialized with " << v_num << " vertex entries.\n" << std::endl;
}

digraph_vector_t Digraph::get_digraph_vector() {
    return topology;
}

vertex_bitset_t Digraph::get_vertex_index() {
    // TODO: Check wheter to reterun a reference here instead.
    return vertex_index;
}

void Digraph::print_edges() {
    for (std::vector<Dvertex>::size_type v = 0; v != topology.size(); v++) {
        std::cout << "v" << v << ": " << std::endl;
        for (vertex_id_t in_neighbor : *topology[v].in_neighbors) {
            std::cout << in_neighbor << " " << std::endl;
        }
        for (vertex_id_t out_neighbor : *topology[v].out_neighbors) {
            std::cout << out_neighbor << " " << std::endl;
        }
        std::cout << std::endl;
    std::cout << std::endl;
    }
}

bool Digraph::has_vertex(vertex_id_t v) {
    return vertex_index[v];
}

bool Digraph::has_edge(vertex_id_t src_v, vertex_id_t dst_v) {
    // std::find() has a worst-case time of O(n) in the distance between first and last.

    // TODO: Remove duplicated neighbors vectors and implement a new way of checking for existance of the
    // edges that are in the opposite direction of those represented by neighbor vectors
    return std::find(topology[src_v].out_neighbors->begin(), topology[src_v].out_neighbors->end(), dst_v)
           != topology[src_v].out_neighbors->end();
}

void Digraph::add_edge(vertex_id_t src_v, vertex_id_t dst_v) {
    if (!has_edge(src_v, dst_v)) {
        // std::vector.push_back() has a O(1) amortized time.
        // Reallocation may happen.
        // If a reallocation happens, the reallocation is O(n).

        if(!has_vertex(src_v)){
            vertex_index[src_v] = 1;
            increment_order();
        }

        if(!has_vertex(dst_v)){
            vertex_index[dst_v] = 1;
            increment_order();
        }

        // TODO: Remove duplicated neighbors vectors, store just one type of neighbors
        topology[src_v].out_neighbors->push_back(dst_v);
        topology[dst_v].in_neighbors->push_back(src_v);

        topology[src_v].out_degree++;
        topology[dst_v].in_degree++;

        increment_size();

        touched_src_verts.push_back(src_v);
    }
}

void Digraph::add_edge_populate(vertex_id_t src_v, vertex_id_t dst_v) { // JUST FOR TESTING!
    if (!has_edge(src_v, dst_v)) {
        if(!has_vertex(src_v)){
            vertex_index[src_v] = 1;
            increment_order();
        }

        if(!has_vertex(dst_v)){
            vertex_index[dst_v] = 1;
            increment_order();
        }

        // TODO: Remove duplicated neighbors vectors, store just one type of neighbors
        topology[src_v].out_neighbors->push_back(dst_v);
        topology[dst_v].in_neighbors->push_back(src_v);

        topology[src_v].out_degree++;
        topology[dst_v].in_degree++;

        increment_size();
    }
}

void Digraph::remove_edge(vertex_id_t src_v, vertex_id_t dst_v) {
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
            decrement_order();
        } else if(get_out_degree(src_v) > 0) {
            touched_src_verts.push_back(src_v);
        }

        if(get_out_degree(dst_v) == 0 && get_in_degree(dst_v) == 0) {
            vertex_index[dst_v] = 0;
            decrement_order();
        } else if(get_out_degree(dst_v) > 0) {
            touched_src_verts.push_back(dst_v);
        }


    }
}

neighbors_vector_t *Digraph::get_in_neighborhood(vertex_id_t v) {
    // TODO: Remove duplicated neighbors vectors
    return topology[v].in_neighbors;
}

neighbors_vector_t *Digraph::get_out_neighborhood(vertex_id_t v) {
    return topology[v].out_neighbors;
}

graph_size_t Digraph::get_in_degree(vertex_id_t v) {
    // TODO: Finish the and test the implementation of degree counting
    return topology[v].in_degree;
    //return static_cast<graph_size_t>(get_in_neighborhood(v)->size());
}

graph_size_t Digraph::get_out_degree(vertex_id_t v) {
    // TODO: Finish the and test the implementation of degree counting
    return topology[v].out_degree;
    //return static_cast<graph_size_t>(get_out_neighborhood(v)->size());
}

graph_size_t Digraph::get_degree(vertex_id_t v) {
    return get_in_degree(v) + get_out_degree(v);
}

void Digraph::update_state(vertex_id_t v, state_t state_new) {
    states_temp[v] = state_new;

    if(std::abs(state_new - states[v]) >= state_change_tolerance){
        state_change_monitor = true;
    }

    //std::cout << "Old state of vertex " << v << " is " << states[v] << std::endl;
    //std::cout << "New state of vertex " << v << " is " << states_temp[v] << std::endl;
}

state_t Digraph::get_state(vertex_id_t v) {
    return states[v];
}

// izbrisi ovo
void Digraph::finalize_state(vertex_id_t v) {
        states[v] = states_temp[v];
}

void Digraph::finalize_states() {
    states_temp.swap(states);
}

void Digraph::set_state_change_tolerance(state_t epsilon){
    state_change_tolerance = epsilon;
}

bool Digraph::state_changed() {
    return state_change_monitor;
}

void Digraph::set_state_change() {
    state_change_monitor = true;
}

void Digraph::unset_state_change() {
    state_change_monitor = false;
}

vertex_queue_t *Digraph::get_touched_src_verts(){
    return &touched_src_verts;
}

void Digraph::reset_touched_src_verts(){
    touched_src_verts.clear();
}

vertex_bitset_t *Digraph::get_visited_verts(){
    return &visited_verts;
}

void Digraph::set_visited(vertex_id_t v){
    visited_verts[v] = 1;
}

void Digraph::unset_visited(vertex_id_t v){
    visited_verts[v] = 0;
}

void Digraph::reset_visited_verts(){
    visited_verts.reset();
}

bool Digraph::has_been_visited(vertex_id_t v){
    return visited_verts[v];
}

void Digraph::count_order() {
    // Obsolete function.. Order is always incrementally maintained.
    order = static_cast<graph_size_t>(vertex_index.count());
}

graph_size_t Digraph::get_order() {
    return order;
}

graph_size_t Digraph::get_max_order() {
    return max_vertex_allocations;
}

void Digraph::increment_order() {
    order++;
}

void Digraph::decrement_order() {
    order--;
}

graph_size_t Digraph::get_size() {
    return size;
}

void Digraph::increment_size() {
    size++;
}

void Digraph::decrement_size() {
    size--;
}


