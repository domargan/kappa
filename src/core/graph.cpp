#include <iostream>
#include <algorithm>

#include "graph.h"
#include "vertex.h"

Graph::Graph(unsigned int v_num) {
    std::cout << "Graph constructor called.\n" << std::endl;

    max_vertex_number = v_num;
    // std::vector.assign() has O(n) in this case.
    //topology.assign(max_vertex_number, uvertex());
    for (int i=0; i<max_vertex_number; i++) {
        uvertex uv;
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
    //for (auto &edge : edges) {
    //    for (int i : edge) {
    for (int i=0; i<max_vertex_number; i++){
        std::cout << "v" << i << ": " << std::endl;
        //for (auto j : topology[i].neighbors){
        for (int j = 0; j < topology[i].neighbors.size(); j++) {
            std::cout << topology[i].neighbors[j] << " " << std::endl;
            //std::cout << j << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Graph::has_edge(unsigned int src_v, unsigned int dst_v){
    // std::find() has a worst-case time of O(n) in the distance between first and last.
    return std::find(topology[src_v].neighbors.begin(), topology[src_v].neighbors.end(), dst_v)
           != topology[src_v].neighbors.end();
}

void Graph::add_edge(unsigned int src_v, unsigned int dst_v) {
    // std::vector.push_back() has a O(1) amortized time.
    // Reallocation may happen.
    // If a reallocation happens, the reallocation is O(n).
    topology[src_v].neighbors.push_back(dst_v);
    //topology[src_v];
}

void Graph::remove_edge(unsigned int src_v, unsigned int dst_v) {
    // Erase-remove has a worst-case time of O(n).
    //edges[src_v].erase(std::remove(edges[src_v].begin(), edges[src_v].end(), dst_v), edges[src_v].end());;
    topology[src_v].neighbors.erase(std::remove(topology[src_v].neighbors.begin(), topology[src_v].neighbors.end(), dst_v), topology[src_v].neighbors.end());
}


neighbors_vector Graph::get_neighborhood(unsigned int v) {
    return topology[v].neighbors;
}

unsigned int Graph::get_degree(unsigned int v) {
    return get_neighborhood(v).size();
}

unsigned int Graph::get_order() {
    return 0;
}

unsigned int Graph::get_size() {
    return 0;
}

void Graph::increment_order() {

}

void Graph::decrement_order() {

}

void Graph::increment_size() {

}

void Graph::decrement_size() {

}
