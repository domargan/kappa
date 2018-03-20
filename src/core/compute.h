#ifndef KAPPA_COMPUTE_H
#define KAPPA_COMPUTE_H

#include "digraph.h"

const float DEFAULT_CHANGE_TOLERANCE = 0.0001;
const int DEFAULT_MAX_ITERATIONS = 20;

void run_global(Digraph*, void(*)(uint32_t, Digraph*));
void run_local(Digraph*, void(*)(uint32_t, Digraph*));

#endif //KAPPA_COMPUTE_H
