#include <iostream>
#include <map>

#include "digraph.h"
#include "pagerank.h"

float damping_factor = DEFAULT_DAMPING_FACTOR;
int max_iterations = DEFAULT_MAX_ITERATIONS;

void init_pr_values(Digraph* g){
    g->count_order();

    float pr_init_val = (1.0 / g->get_order());

    std::cout << "Initial PageRank value is " << pr_init_val << std::endl;

    unsigned int max_order = g->get_max_order();

    for (unsigned int i = 0; i < max_order; i++) {
        g->update_state(i, pr_init_val);
    }

    g->finalize_states();
}

void pr_compute_single_vertex(unsigned int v, Digraph* g) {
    float pr_neighbourhood_sum = 0.0;
    for (auto neighbour : *(g->get_in_neighborhood(v))){
        if(g->get_out_degree(neighbour) > 0) {
            pr_neighbourhood_sum += g->get_state(neighbour) / g->get_out_degree(neighbour);
        }
    }

    float pr = ((1.0 - damping_factor) / g->get_order()) + (damping_factor * pr_neighbourhood_sum);
    g->update_state(v, pr);
}

void pr_compute(Digraph* g){
    g->count_order();

    // Note: Don't forget to set initial PageRank vertex states outside (before) this function!

    std::cout << "Starting PageRank computation (" << max_iterations << " iterations)..." << std::endl;
    int iterations = 0;

    unsigned int max_order = g->get_max_order();

    while(iterations++ < max_iterations) {
        for (unsigned int i = 0; i < max_order; i++) {
            if(g->has_vertex(i)){
                pr_compute_single_vertex(i, g);
            }
        }

        g->finalize_states();
    }
}