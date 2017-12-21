#include <iostream>
#include <algorithm>

#include "graph.h"

Graph::Graph(unsigned int v_num) {
    std::cout << "Graph constructor called.\n" << std::endl;

    max_vertex_number = v_num;

    for (int i = 0; i < max_vertex_number; i++) {
        uvertex uv{};
        uv.neighbors = new neighbors_vector;
        uv.degree = 0;
        uv.state = 0;
        uv.state_temp = 0;

        topology.push_back(uv);
    }

    std::cout << "Graph structure initialized with " << v_num << " vertex entries.\n" << std::endl;

    order = 0;
    size = 0;
}

graph_vector Graph::get_graph_vector() {
    return topology;
}

void Graph::print_edges() {
    for (std::vector<uvertex>::size_type v = 0; v != topology.size(); v++) {
        std::cout << "v" << v << ": " << std::endl;
        for (unsigned int neighbor : *topology[v].neighbors) {
            std::cout << neighbor << " " << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Graph::has_edge(unsigned int src_v, unsigned int dst_v) {
    // std::find() has a worst-case time of O(n) in the distance between first and last.
    return std::find(topology[src_v].neighbors->begin(), topology[src_v].neighbors->end(), dst_v)
           != topology[src_v].neighbors->end();
}

void Graph::add_edge(unsigned int src_v, unsigned int dst_v) {
    if (!has_edge(src_v, dst_v)) {
        // std::vector.push_back() has a O(1) amortized time.
        // Reallocation may happen.
        // If a reallocation happens, the reallocation is O(n).
        topology[src_v].neighbors->push_back(dst_v);
        topology[dst_v].neighbors->push_back(src_v);
    }
}

void Graph::remove_edge(unsigned int src_v, unsigned int dst_v) {
    if (has_edge(src_v, dst_v)) {
        // Erase-remove has a worst-case time of O(n).
        topology[src_v].neighbors->erase
                (std::remove(topology[src_v].neighbors->begin(), topology[src_v].neighbors->end(), dst_v),
                 topology[src_v].neighbors->end());

        topology[dst_v].neighbors->erase
                (std::remove(topology[dst_v].neighbors->begin(), topology[dst_v].neighbors->end(), src_v),
                 topology[dst_v].neighbors->end());
    }
}

neighbors_vector Graph::get_neighborhood(unsigned int v) {
    return *topology[v].neighbors;
}

unsigned int Graph::get_degree(unsigned int v) {
    return static_cast<unsigned int>(get_neighborhood(v).size());
}

unsigned int Graph::get_order() {
    return order;
}

unsigned int Graph::get_size() {
    return size;
}

void Graph::increment_order() {
    order++;
}

void Graph::decrement_order() {
    order--;
}

void Graph::increment_size() {
    size++;
}

void Graph::decrement_size() {
    size--;
}
