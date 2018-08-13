#ifndef KAPPA_COMPUTE_H
#define KAPPA_COMPUTE_H

#include "update.h"

class Digraph;

typedef struct Computation {
    void (*init)(Digraph*, vertex_id_t);
    void (*incr_compute)(Digraph*, Update*);
} Computation;

#endif //KAPPA_COMPUTE_H
