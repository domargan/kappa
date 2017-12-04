#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include <iostream>
#include <set>

// edge_vector is the main data structure for our graph: a std::vector of std::sets of integers.
// std::set is implemented as a red-black binary search tree.
// Outer vector's indices represents source vertices' IDs.
// Inner sets hold destination vertices' IDs for each source vertex.
typedef std::vector<std::set<unsigned int>> edge_vector;

class Graph {
private:
    unsigned int max_vertex_number;
    edge_vector edges;
public:
    explicit Graph(unsigned int);
    edge_vector get_edge_vector();
    void print_edge_vector();
    void add_edge(unsigned int, unsigned int);
    void remove_edge(unsigned int, unsigned int);
    bool edge_exists(unsigned int src_v, unsigned int dst_v);
};


#endif //KAPPA_GRAPH_H
