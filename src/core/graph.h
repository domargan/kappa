//
// Created by dm1515 on 07/11/17.
//

#ifndef KAPPA_GRAPH_H
#define KAPPA_GRAPH_H

#include <vector>
#include <iostream>

typedef std::vector<std::vector<int>> edge_array;

class Graph {
private:
    edge_array edges;
public:
    explicit Graph(int);
    edge_array get_edge_array();
};


#endif //KAPPA_GRAPH_H
