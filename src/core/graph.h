//
// Created by dm1515 on 07/11/17.
//

#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include <iostream>

typedef std::vector<std::vector<int>> edge_vector;

class Graph {
private:
    edge_vector edges;
public:
    explicit Graph(unsigned long);
    edge_vector get_edge_vector();
    void print_edge_vector();
    void add_edge(int, int);
    void remove_edge(int, int);
};


#endif //KAPPA_GRAPH_H
