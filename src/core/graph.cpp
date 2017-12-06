#include "graph.h"
#include "algorithm"

Graph::Graph(unsigned int v_num) {
    std::cout << "Graph constructor called.\n" << std::endl;

    max_vertex_number = v_num;
    // std::vector.assign() has O(n) in this case.
    edges.assign(max_vertex_number, std::vector<unsigned int>());

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
    // std::find() has a worst-case time of O(n) in the distance between first and last.
    return std::find(edges[src_v].begin(), edges[src_v].end(), dst_v) != edges[src_v].end();
}

void Graph::add_edge(unsigned int src_v, unsigned int dst_v) {
    // std::vector.push_back() has a O(1) amortized time.
    // Reallocation may happen.
    // If a reallocation happens, the reallocation is O(n).
    edges[src_v].push_back(dst_v);
}

void Graph::remove_edge(unsigned int src_v, unsigned int dst_v) {
    // Erase-remove has a worst-case time of O(n).
    edges[src_v].erase(std::remove(edges[src_v].begin(), edges[src_v].end(), dst_v), edges[src_v].end());;
}

int Graph::in_degree(unsigned int) {
    return 0;
}

int Graph::out_degree(unsigned int) {
    return 0;
}

int Graph::degree(unsigned int) {
    return 0;
}
