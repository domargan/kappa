#include <iostream>
#include <map>

#include "digraph.h"
#include "pagerank.h"

void init_pr_values(Digraph* g){
    g->count_order();

    float pr_init_val = (1.0 / g->get_order());

    std::cout << "Initial PageRank value is " << pr_init_val << std::endl;

    uint32_t max_order = g->get_max_order();

    for (uint32_t i = 0; i < max_order; i++) {
        g->update_state(i, pr_init_val);
    }

    g->finalize_states();
}

void pr_compute_single_vertex(uint32_t v, Digraph* g) {
    float pr_neighbourhood_sum = 0.0;
    for (auto neighbour : *(g->get_in_neighborhood(v))){
        if(g->get_out_degree(neighbour) > 0) {
            pr_neighbourhood_sum += g->get_state(neighbour) / g->get_out_degree(neighbour);
        }
    }

    float pr = ((1.0 - DEFAULT_DAMPING_FACTOR) / g->get_order()) + (DEFAULT_DAMPING_FACTOR * pr_neighbourhood_sum);
    g->update_state(v, pr);
}

void pr_compute(Digraph* g){
    g->set_state_change_tolerance(DEFAULT_CONVERGENCE);

    g->count_order();

    // Note: Don't forget to set initial PageRank vertex states outside (before) this function!

    std::cout << "Starting PageRank computation (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    uint32_t max_order = g->get_max_order();

    int num_iterations = 0;
    g->state_change_monitor = true;

    while(g->state_change_monitor && num_iterations < DEFAULT_MAX_ITERATIONS) {
        g->state_change_monitor = false;

        for (uint32_t i = 0; i < max_order; i++) {
            if(g->has_vertex(i)){
                pr_compute_single_vertex(i, g);
            }
        }

        g->finalize_states();

        num_iterations++;
    }

    if(num_iterations == DEFAULT_MAX_ITERATIONS) {
        std::cout << "PageRank stopped after " << num_iterations << " iterations" << std::endl;
    } else {
        std::cout << "PageRank converged after " << num_iterations << " iterations" << std::endl;
    }
}