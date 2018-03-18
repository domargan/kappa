#include <iostream>
#include "compute.h"

// TODO: Implement compute() as a function of vertex struct.
// User then overrides the compute() function and it can be used here as
// vertex.compute() instead of passing it as a callback function.

// TODO: Check Pregel's Compute() again and see whether it makes sense to implement everything as vertex operators

// TODO: Make this more general (this is too PageRank oriented)

void run(Digraph* g, void(*vertex_compute)(uint32_t, Digraph*)) {
    //std::cout << "Starting computations (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    uint32_t max_order = g->get_max_order();

    int num_iterations = 0;
    g->state_change_monitor = true;

    while(g->state_change_monitor && num_iterations < DEFAULT_MAX_ITERATIONS) {
        g->state_change_monitor = false;

        // TODO: Replace this with localised computation!
        for (uint32_t i = 0; i < max_order; i++) {
            if(g->has_vertex(i)){
                vertex_compute(i, g);
            }
        }

        g->finalize_states();

        num_iterations++;
    }

    /*
    if(num_iterations == DEFAULT_MAX_ITERATIONS) {
        std::cout << "Computation stopped after " << num_iterations << " iterations" << std::endl;
    } else {
        std::cout << "Computation converged after " << num_iterations << " iterations" << std::endl;
    }
    */
}
