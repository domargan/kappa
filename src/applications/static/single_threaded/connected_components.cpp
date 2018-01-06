#include <iostream>
#include <vector>

#include "connected_components.h"
#include "dfs.h"

std::vector<unsigned int> components;

void cc_compute(Graph* g){
    int number_of_components = 0;

    for(int i=0; i<g->get_order(); i++){
        if(g->get_state(i) != 1) {
            dfs(g, i);

            number_of_components++;
        }
    }

    std::cout << "Number of components: " << number_of_components << std::endl;
}
