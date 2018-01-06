#include <iostream>

#include <boost/dynamic_bitset.hpp>

#include "dfs.h"

void dfs(Digraph* g, unsigned int v) {
    std::cout << "Visiting vertex " << v << "..." << std::endl;
    g->update_state(v, 1);
    g->finalize_state(v);

    for(auto neighbor : g->get_out_neighborhood(v)) {
        if(g->get_state(neighbor) != 1) {
            dfs(g, neighbor);
        }
    }
}

void dfs_compute(Digraph* g, unsigned int v) {
    std::cout << "Starting DFS computations..." << std::endl;
    if(g->has_vertex(v)){
        dfs(g, v);
    }
}