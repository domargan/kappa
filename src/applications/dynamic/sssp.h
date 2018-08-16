#ifndef KAPPA_SSSP_H
#define KAPPA_SSSP_H

#include "digraph.h"

namespace SSSP {
    void init(Digraph*, vertex_id_t);
    void on_add_edge(Digraph*, vertex_id_t, vertex_id_t);
    void on_remove_edge(Digraph*, vertex_id_t, vertex_id_t);
}

#endif //KAPPA_SSSP_H
