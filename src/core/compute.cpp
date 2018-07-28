#include <iostream>
#include "compute.h"
#include "thread_pool.hpp"

// TODO: Implement compute() as a function of vertex struct.
// User then overrides the compute() function and it can be used here as
// vertex.compute() instead of passing it as a callback function.

// TODO: Check Pregel's Compute() again and see whether it makes sense to implement everything as vertex operators

// TODO: Make this more general (this is too PageRank oriented)

// TODO: Use references in for loops

void run_global(Digraph *g, void (*vertex_compute)(vertex_id_t, Digraph *)) {
    //std::cout << "Starting computations (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    graph_size_t max_order = g->get_max_order();

    int num_iterations = 0;
    //g->state_change_monitor = true;
    g->set_state_change();

    //while(g->state_change_monitor && num_iterations < DEFAULT_MAX_ITERATIONS) {
    while (g->state_changed() && num_iterations < DEFAULT_MAX_ITERATIONS) {
        //g->state_change_monitor = false;
        g->unset_state_change();

        for (graph_size_t i = 0; i < max_order; i++) {
            if (g->has_vertex(i)) {
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

void dfs_local_compute(Digraph *g, vertex_id_t v, void (*vertex_compute)(vertex_id_t, Digraph *), ThreadPool &workerPool) {
    // TODO: negdje je zajeb, nadji di... PRINTA NULE.. ILI mozda vise ne :)
    //std::cout << "Visiting vertex " << v << "..." << std::endl;
    g->set_visited(v);

    // vertex_compute(v, g);
    workerPool.submit(vertex_compute, v, g);

    // TODO: if difference in the states for v is less than epsion, don't go deeper with dfs
    for (auto neighbor : *(g->get_out_neighborhood(v))) {
        if (!g->has_been_visited(v)) {
            dfs_local_compute(g, neighbor, vertex_compute, workerPool);
        }
    }
}

void run_local(Digraph *g, void (*vertex_compute)(vertex_id_t, Digraph *), ThreadPool &workerPool) {
    //std::cout << "Starting computations (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    int num_iterations = 0;
    //g->state_change_monitor = true;
    g->set_state_change();

    vertex_queue_t *touched_src_vets = g->get_touched_src_verts();

    //while(g->state_change_monitor && num_iterations < DEFAULT_MAX_ITERATIONS) {
    while (g->state_changed() && num_iterations < DEFAULT_MAX_ITERATIONS) {
        //g->state_change_monitor = false;
        g->unset_state_change();

        for (auto v : *touched_src_vets) {
            if (!g->has_been_visited(v)) {
                dfs_local_compute(g, v, vertex_compute, workerPool);
            }
        }

        g->reset_visited_verts();

        g->finalize_states();

        num_iterations++;
    }

    g->reset_touched_src_verts();

    /*
    if(num_iterations == DEFAULT_MAX_ITERATIONS) {
        std::cout << "Computation stopped after " << num_iterations << " iterations" << std::endl;
    } else {
        std::cout << "Computation converged after " << num_iterations << " iterations" << std::endl;
    }
     */
}
