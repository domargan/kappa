#include "pagerank.h"

namespace PageRank {
    void init(Digraph *g, vertex_id_t v) {
        float init_state = 1.0 / g->get_order();

        g->set_state(v, init_state);
    }

    void propagate(Digraph *g, vertex_id_t v) {
        float shares = 0.0;

        for (auto neighbour : *(g->get_in_neighborhood(v))) {
            graph_size_t out_degree = g->get_out_degree(neighbour);

            if (out_degree > 0) {
                shares += g->get_state(neighbour) / out_degree;
            }
        }

        float new_pr = ((1.0 - DAMPING_FACTOR) / g->get_order()) + (DAMPING_FACTOR * shares);

        float old_pr = g->get_state(v);
        g->set_state(v, new_pr);

        if (std::abs(new_pr - old_pr) >= EPSILON) {
            for (auto neighbour : *(g->get_out_neighborhood(v))) {
                propagate(g, neighbour);
            }
        }
    }

    void incr_compute(Digraph *g, Update *u) {
        for (auto neighbour : *(g->get_out_neighborhood(u->src))) {
            propagate(g, neighbour);
        }
    }
}
