//
// Created by dm1515 on 07/11/17.
//

#include "graph.h"

Graph::Graph(int v_num) {
    edges.assign(v_num, std::vector<int>(0, 0));
}

edge_array Graph::get_edge_array() {
    for (std::size_t i=0; i<edges.size(); i++){
        for (std::size_t j=0; j<edges[i].size(); j++){
            std::cout << edges[i][j] << " ";
        }
        std::cout << std::endl;
    }

    return edge_array();
}
