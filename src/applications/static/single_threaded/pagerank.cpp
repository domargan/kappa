#include <iostream>
#include <map>

#include "digraph.h"
#include "pagerank.h"

double damping_factor = DEFAULT_DAMPING_FACTOR;
int max_iterations = DEFAULT_MAX_ITERATIONS;

void init_pr_values(Digraph* g){
    double pr_init_val = (1.0 / g->get_order());

    std::cout << "Initial PageRank value is " << pr_init_val << std::endl;

    for (unsigned int i = 0; i < g->get_vertex_index().size(); i++) {
        g->update_state(i, pr_init_val);
    }

    g->finalize_states();
}

void pr_compute_single_vertex(unsigned int v, Digraph* g) {
    if (g->has_vertex(v)) {
        std::map<unsigned int, double> neighbors_out_degree_map;

        for (auto neighbour : g->get_out_neighborhood(v)) {
            neighbors_out_degree_map[neighbour] = g->get_out_degree(neighbour);
        }

        double pr_neighbourhood_sum = 0.0;
        for (auto neighbour : g->get_out_neighborhood(v)){
            if(neighbors_out_degree_map[neighbour] > 0) {
                pr_neighbourhood_sum += g->get_state(neighbour) / neighbors_out_degree_map[neighbour];
            }
        }

        double pr = ((1.0 - damping_factor) / g->get_order()) + (damping_factor * pr_neighbourhood_sum);
        g->update_state(v, pr);
    }
}

void pr_compute(Digraph* g){
    std::cout << "Initialising PageRank values... " << std::endl;
    init_pr_values(g);

    std::cout << "Starting PageRank computation (" << max_iterations << " iterations)..." << std::endl;
    int iterations = 0;

    while(iterations++ < max_iterations) {
        for (unsigned int i = 0; i < g->get_vertex_index().size(); i++) {
            if(g->has_vertex(i)){
                pr_compute_single_vertex(i, g);
            }
        }

        g->finalize_states();
    }
}