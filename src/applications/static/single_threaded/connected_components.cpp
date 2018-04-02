#include <iostream>
#include <vector>

#include "connected_components.h"
#include "dfs.h"

std::vector<graph_size_t> components;

void cc_compute(Graph* g){
    int number_of_components = 0;

    graph_size_t order = g->get_order();

    for(int i=0; i<order; i++){
        if(g->get_state(i) != 1) {
            dfs(g, i);

            number_of_components++;
        }
    }

    std::cout << "Number of components: " << number_of_components << std::endl;
}
