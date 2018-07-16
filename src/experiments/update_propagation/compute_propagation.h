#ifndef KAPPA_COMPUTE_H
#define KAPPA_COMPUTE_H

#include "digraph.h"

const state_t DEFAULT_CHANGE_TOLERANCE = 0.00006574;
const int DEFAULT_MAX_ITERATIONS_PROPAGATION = 100000;

void run_local_propagation(Digraph*, void(*)(vertex_id_t, Digraph*));

#endif //KAPPA_COMPUTE_H
