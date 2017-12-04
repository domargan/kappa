#include "graph.h"
#include "algorithm"

Graph::Graph(unsigned int v_num) {
    std::cout << "Graph constructor called.\n" << std::endl;

    max_vertex_number = v_num;
    // std::vector.assign() has O(n) in this case.
    edges.assign(max_vertex_number, std::set<unsigned int>());

    std::cout << "Graph structure initialized with " << v_num << " vertex entries.\n" << std::endl;
}

edge_vector Graph::get_edge_vector() {
    return edge_vector();
}

void Graph::print_edge_vector() {
    //for (auto &edge : edges) {
    //    for (int i : edge) {
    for (int i=0; i<max_vertex_number; i++){
        std::cout << "v" << i << ": ";
        for (auto j : edges[i]){
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool Graph::edge_exists(unsigned int src_v, unsigned  int dst_v){
    // std::set.find() has a worst-case of O(log(n)).
    return edges[src_v].find(dst_v) != edges[src_v].end();
}

void Graph::add_edge(unsigned int src_v, unsigned int dst_v) {
    // std::set.insert() has a worst-case of O(log(n)).
    edges[src_v].insert(dst_v);
}

void Graph::remove_edge(unsigned int src_v, unsigned int dst_v) {
    // std::set.erase() for a single element has a time of amortized O(1).
    edges[src_v].erase(dst_v);
}