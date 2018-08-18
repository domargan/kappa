#ifndef KAPPA_DYNAMIC_PAGERANK_H
#define KAPPA_DYNAMIC_PAGERANK_H

#include "digraph.h"

namespace PageRank {
    const float DAMPING_FACTOR = 0.85;
    const float EPSILON = 0.0001;

    void init_state(Digraph*, vertex_id_t);
    void on_activate(Digraph*, vertex_id_t);
    void on_add_edge(Digraph*, vertex_id_t, vertex_id_t);
    void on_remove_edge(Digraph*, vertex_id_t, vertex_id_t);
}

#endif //KAPPA_DYNAMIC_PAGERANK_H
