#include <iostream>
#include "compute_propagation.h"

void dfs_local_compute_propagation(Digraph* g, vertex_id_t v, void(*vertex_compute)(vertex_id_t, Digraph*), int& count) {
    ++count;

    //std::cout << "Visiting vertex " << v << "..." << std::endl;
    g->set_visited(v);

    vertex_compute(v, g);

    // TODO: if difference in the states for v is less than epsion, don't go deeper with dfs
    for(auto neighbor : *(g->get_out_neighborhood(v))) {
        if(!g->has_been_visited(v)){
            dfs_local_compute_propagation(g, neighbor, vertex_compute, count);
        }
    }
}

void run_local_propagation(Digraph* g, void(*vertex_compute)(vertex_id_t, Digraph*)) {
    //std::cout << "Starting computations (maximum " << DEFAULT_MAX_ITERATIONS << " iterations)..." << std::endl;

    int num_iterations = 0;
    g->set_state_change();

    vertex_queue_t* touched_src_vets = g->get_touched_src_verts();

    while(g->state_changed() && num_iterations < DEFAULT_MAX_ITERATIONS_PROPAGATION) {
        g->unset_state_change();

        //int temp_touched_count = 0;

        for(auto v : *touched_src_vets){
            //temp_touched_count++;
            if(!g->has_been_visited(v)) {
                int propagation_count = 0;

                dfs_local_compute_propagation(g, v, vertex_compute, propagation_count);

                //std::cout << "Propagation for vertex " << v << ": " << propagation_count << std::endl;
            }
        }
        //std::cout << "Temp touched verts count: " << temp_touched_count << std::endl;

        //std::cout << "Visited verts before reset: " << g->get_visited_verts()->count() << std::endl;

        g->reset_visited_verts();

        //std::cout << "Visited verts after reset: " << g->get_visited_verts()->count() << std::endl;

        g->finalize_states();

        num_iterations++;
    }

    g->reset_touched_src_verts();



    if(num_iterations == DEFAULT_MAX_ITERATIONS_PROPAGATION) {
        std::cout << "Computation stopped after " << num_iterations << " iterations" << std::endl;
    } else {
        std::cout << "Computation converged after " << num_iterations << " iterations" << std::endl;
    }

}
