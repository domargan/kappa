#ifndef KAPPA_COMPUTE_H
#define KAPPA_COMPUTE_H

#include "digraph.h"
#include "thread_pool.hpp"

const float DEFAULT_CHANGE_TOLERANCE = 0.0001;
const int DEFAULT_MAX_ITERATIONS = 20;

void run_global(Digraph*, void(*)(vertex_id_t, Digraph*));
void run_local(Digraph*, void(*)(vertex_id_t, Digraph*), ThreadPool&);

#endif //KAPPA_COMPUTE_H
