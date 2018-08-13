#ifndef KAPPA_DYNAMIC_PAGERANK_H
#define KAPPA_DYNAMIC_PAGERANK_H

#include "digraph.h"

namespace PageRank {
    const float DAMPING_FACTOR = 0.85;
    const float EPSILON = 0.0001;

    void init(Digraph*, vertex_id_t);
    void incr_compute(Digraph*, Update*);
}

#endif //KAPPA_DYNAMIC_PAGERANK_H
