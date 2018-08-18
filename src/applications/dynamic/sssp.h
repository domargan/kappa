#ifndef KAPPA_SSSP_H
#define KAPPA_SSSP_H

#include "digraph.h"

namespace SSSP {
    const vertex_id_t SOURCE = 1;

    void init_state(Digraph*, vertex_id_t);
    void on_activate(Digraph*, vertex_id_t);
    void on_add_edge(Digraph*, vertex_id_t, vertex_id_t);
    void on_remove_edge(Digraph*, vertex_id_t, vertex_id_t);
}

#endif //KAPPA_SSSP_H
