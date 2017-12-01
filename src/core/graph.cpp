//
// Created by dm1515 on 07/11/17.
//

#include "graph.h"

Graph::Graph(unsigned long v_num) {
    // Initialize a vector of vectors.
    // Source vertex ID == outer vector's field index.
    // Each field in the outer vector contains a sub-vector of ints, where each int == dest. vertex ID.
    edges.assign(v_num, std::vector<int>(0, 0));
}

edge_vector Graph::get_edge_vector() {
    return edge_vector();
}

void Graph::print_edge_vector() {
    for (auto &edge : edges) {
        for (int i : edge) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}

void Graph::add_edge(int, int) {

}

void Graph::remove_edge(int, int) {

}
