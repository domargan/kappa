#include <iostream>
#include <algorithm>

#include "digraph.h"

Digraph::Digraph(unsigned int v_num) {
    std::cout << "Digraph constructor called.\n" << std::endl;

    max_vertex_number = v_num + 1;

    vertex_index = boost::dynamic_bitset<>(max_vertex_number);

    for (int i = 0; i < max_vertex_number; i++) {
        dvertex dv{};
        dv.in_neighbors = new neighbors_vector;
        dv.out_neighbors = new neighbors_vector;
        dv.in_degree = 0;
        dv.out_degree = 0;
        dv.state = 0;
        dv.state_temp = 0;

        topology.push_back(dv);
    }

    std::cout << "Diraph structure initialized with " << v_num << " vertex entries.\n" << std::endl;

    order = 0;
    size = 0;
}

digraph_vector Digraph::get_digraph_vector() {
    return topology;
}

vertex_bitset Digraph::get_vertex_index() {
    return vertex_index;
}

void Digraph::print_edges() {
    for (std::vector<dvertex>::size_type v = 0; v != topology.size(); v++) {
        std::cout << "v" << v << ": " << std::endl;
        for (unsigned int in_neighbor : *topology[v].in_neighbors) {
            std::cout << in_neighbor << " " << std::endl;
        }
        for (unsigned int out_neighbor : *topology[v].out_neighbors) {
            std::cout << out_neighbor << " " << std::endl;
        }
        std::cout << std::endl;
    std::cout << std::endl;
    }
}

bool Digraph::has_vertex(unsigned int vertex_id) {
    return vertex_index[vertex_id];
}

bool Digraph::has_edge(unsigned int src_v, unsigned int dst_v) {
    // std::find() has a worst-case time of O(n) in the distance between first and last.
    return std::find(topology[src_v].out_neighbors->begin(), topology[src_v].out_neighbors->end(), dst_v)
           != topology[src_v].out_neighbors->end();
}

void Digraph::add_edge(unsigned int src_v, unsigned int dst_v) {
    if (!has_edge(src_v, dst_v)) {
        // std::vector.push_back() has a O(1) amortized time.
        // Reallocation may happen.
        // If a reallocation happens, the reallocation is O(n).
        topology[src_v].out_neighbors->push_back(dst_v);
        topology[dst_v].in_neighbors->push_back(src_v);

        vertex_index[src_v] = 1;
        vertex_index[dst_v] = 1;

        increment_size();
    }
}

void Digraph::remove_edge(unsigned int src_v, unsigned int dst_v) {
    if (has_edge(src_v, dst_v)) {
        // Erase-remove has a worst-case time of O(n).
        topology[src_v].out_neighbors->erase
                (std::remove(topology[src_v].out_neighbors->begin(), topology[src_v].out_neighbors->end(), dst_v),
                 topology[src_v].out_neighbors->end());

        topology[dst_v].in_neighbors->erase
                (std::remove(topology[dst_v].in_neighbors->begin(), topology[dst_v].in_neighbors->end(), src_v),
                 topology[dst_v].in_neighbors->end());

        decrement_size();
    }
}


neighbors_vector Digraph::get_in_neighborhood(unsigned int v) {
    return *topology[v].in_neighbors;
}

neighbors_vector Digraph::get_out_neighborhood(unsigned int v) {
    return *topology[v].out_neighbors;
}

unsigned int Digraph::get_in_degree(unsigned int v) {
    return static_cast<unsigned int>(get_in_neighborhood(v).size());
}

unsigned int Digraph::get_out_degree(unsigned int v) {
    return static_cast<unsigned int>(get_out_neighborhood(v).size());
}

unsigned int Digraph::get_degree(unsigned int v) {
    return get_in_degree(v) + get_out_degree(v);
}

unsigned int Digraph::get_order() {
    return order;
}

unsigned int Digraph::get_size() {
    return size;
}

void Digraph::increment_order() {
    order++;
}

void Digraph::decrement_order() {
    order--;
}

void Digraph::increment_size() {
    size++;
}

void Digraph::decrement_size() {
    size--;
}
