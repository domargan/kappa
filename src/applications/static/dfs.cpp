#include <boost/dynamic_bitset.hpp>
#include <iostream>

#include "dfs.h"

// TODO: Use templates instead of function overloading

/*
void dfs(Digraph* g, vertex_id_t v) {
    std::cout << "Visiting vertex " << v << "..." << std::endl;
    g->update_state(v, 1);
    g->finalize_state(v);

    for(auto neighbor : *(g->get_out_neighborhood(v))) {
        if(g->get_state(neighbor) != 1) {
            dfs(g, neighbor);
        }
    }
}

void dfs_compute(Digraph* g, vertex_id_t v) {
    std::cout << "Starting DFS computations..." << std::endl;
    if(g->has_vertex(v)){
        dfs(g, v);
    }
}
*/
