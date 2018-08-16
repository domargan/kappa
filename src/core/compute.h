#ifndef KAPPA_COMPUTE_H
#define KAPPA_COMPUTE_H

#include "datatypes.h"

class Digraph;

typedef struct Computation {
    void (*init)(Digraph*, vertex_id_t);
    void (*on_add_edge)(Digraph*, vertex_id_t, vertex_id_t);
    void (*on_remove_edge)(Digraph*, vertex_id_t, vertex_id_t);
} Computation;

#endif //KAPPA_COMPUTE_H
