#include    <iostream>
#include <map>

#include "digraph.h"
#include "pagerank_propagation.h"

// TODO: Implement incremental PR init value assignment for new vertices
// TODO: E.g. Is the considerd graph order for init values as large as the max vertex num or as the current temporary order?

void pr_compute_single_vertex_propagation(vertex_id_t v, Digraph* g) {
    float pr_neighbourhood_sum = 0.0;
    for (auto neighbour : *(g->get_in_neighborhood(v))){
        graph_size_t out_degree = g->get_out_degree(neighbour);

        if(out_degree > 0) {
            pr_neighbourhood_sum += g->get_state(neighbour) / out_degree;
        }
    }

    float pr = ((1.0 - DEFAULT_DAMPING_FACTOR) / g->get_order()) + (DEFAULT_DAMPING_FACTOR * pr_neighbourhood_sum);

    g->update_state(v, pr);
}

/*
 * OBSOLETE!
 * Now doing this as part of run() in compute.cpp

void pr_compute_propagation(Digraph* g){
    g->set_state_change_tolerance(DEFAULT_CONVERGENCE);

    //g->count_order();

    // Note: Don't forget to set initial PageRank vertex states outside (before) this function!

    //std::cout << "Starting PageRank computation (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    graph_size_t max_order = g->get_max_order();

    int num_iterations = 0;
    g->state_change_monitor = true;

    while(g->state_change_monitor && num_iterations < DEFAULT_MAX_ITERATIONS) {
        g->state_change_monitor = false;

        for (graph_size_t i = 0; i < max_order; i++) {
            if(g->has_vertex(i)){
                pr_compute_single_vertex(i, g);
            }
        }

        g->finalize_states();

        num_iterations++;
    }

    if(num_iterations == DEFAULT_MAX_ITERATIONS) {
    //    std::cout << "PageRank stopped after " << num_iterations << " iterations" << std::endl;
    } else {
    //    std::cout << "PageRank converged after " << num_iterations << " iterations" << std::endl;
    }
}
 */
