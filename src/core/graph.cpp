#include <iostream>
#include <algorithm>

#include "graph.h"

// TODO: Implement vertex iterator (instead of iterating until vertex_index.size())
// TODO: Do something smarter about getting size; do not call vertex_index.size() all the time
// TODO: For every iteration of vertices from 0 to vertex_index.size() check if the vertex exists before any operations
// TODO: Remove all calls to vertex_index.size() and replace them with a varible
// TODO: FIx neighbors_vector *Graph::get_neighborhood(vertex_id_t v) so it returns a value, not a pointer


Graph::Graph(graph_size_t v_num, state_t init_state) {
    std::cout << "Graph constructor called.\n" << std::endl;

    max_vertex_allocations = v_num + 1;

    vertex_index = boost::dynamic_bitset<>(max_vertex_allocations);

    for (int i = 0; i < max_vertex_allocations; i++) {
        Uvertex uv{};
        uv.neighbors = new neighbors_vector_t;
        uv.state = init_state;
        uv.state_temp = init_state;

        topology.push_back(uv);
    }

    order = 0;
    size = 0;

    std::cout << "Graph structure initialized with " << v_num << " vertex entries.\n" << std::endl;
}

graph_vector_t Graph::get_graph_vector() {
    return topology;
}

vertex_bitset_t Graph::get_vertex_index() {
    return vertex_index;
}

void Graph::print_edges() {
    for (std::vector<Uvertex>::size_type v = 0; v != topology.size(); v++) {
        std::cout << "v" << v << ": " << std::endl;
        for (vertex_id_t neighbor : *topology[v].neighbors) {
            std::cout << neighbor << " " << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Graph::has_vertex(vertex_id_t v) {
    return vertex_index[v];
}

bool Graph::has_edge(vertex_id_t src_v, vertex_id_t dst_v) {
    // std::find() has a worst-case time of O(n) in the distance between first and last.
    return std::find(topology[src_v].neighbors->begin(), topology[src_v].neighbors->end(), dst_v)
           != topology[src_v].neighbors->end();
}

void Graph::add_edge(vertex_id_t src_v, vertex_id_t dst_v) {
    if (!has_edge(src_v, dst_v)) {
        // std::vector.push_back() has a O(1) amortized time.
        // Reallocation may happen.
        // If a reallocation happens, the reallocation is O(n).
        topology[src_v].neighbors->push_back(dst_v);
        topology[dst_v].neighbors->push_back(src_v);

        vertex_index[src_v] = 1;
        vertex_index[dst_v] = 1;

        increment_size();
    }
}

void Graph::remove_edge(vertex_id_t src_v, vertex_id_t dst_v) {
    if (has_edge(src_v, dst_v)) {
        // Erase-remove has a worst-case time of O(n).
        topology[src_v].neighbors->erase
                (std::remove(topology[src_v].neighbors->begin(), topology[src_v].neighbors->end(), dst_v),
                 topology[src_v].neighbors->end());

        topology[dst_v].neighbors->erase
                (std::remove(topology[dst_v].neighbors->begin(), topology[dst_v].neighbors->end(), src_v),
                 topology[dst_v].neighbors->end());

        decrement_size();

        if(get_degree(src_v) == 0) {
            vertex_index[src_v] = 0;
        }

        if(get_degree(dst_v) == 0) {
            vertex_index[dst_v] = 0;
        }
    }
}

neighbors_vector_t *Graph::get_neighborhood(vertex_id_t v) {
    return topology[v].neighbors;
}

graph_size_t Graph::get_degree(vertex_id_t v) {
    return static_cast<graph_size_t>(get_neighborhood(v)->size());
}

void Graph::update_state(vertex_id_t v, state_t state) {
    topology[v].state_temp = state;
}

state_t Graph::get_state(vertex_id_t v) {
    return topology[v].state;
}

void Graph::finalize_state(vertex_id_t v) {
    if(has_vertex(v)){
        topology[v].state = topology[v].state_temp;
    }
}

void Graph::finalize_states() {
    graph_size_t max_order = get_max_order();

    // For each v in the graph exchange state
    for(graph_size_t i=0; i<max_order; i++) {
        finalize_state(i);
    }
}

void Graph::count_order() {
    order = static_cast<graph_size_t>(vertex_index.count());
}

graph_size_t Graph::get_order() {
    return order;
}

graph_size_t Graph::get_max_order() {
    return max_vertex_allocations;
}

graph_size_t Graph::get_size() {
    return size;
}

void Graph::increment_size() {
    size++;
}

void Graph::decrement_size() {
    size--;
}
