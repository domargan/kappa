#ifndef KAPPA_PAGERANK_H
#define KAPPA_PAGERANK_H

#include "digraph.h"

const float DEFAULT_DAMPING_FACTOR = 0.85;

void init_pr_values(Digraph*);
void pr_compute_single_vertex(vertex_id_t, Digraph*);
void pr_compute(Digraph*);

#endif  // KAPPA_PAGERANK_H
