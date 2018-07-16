#ifndef KAPPA_PAGERANK_H
#define KAPPA_PAGERANK_H

const float DEFAULT_DAMPING_FACTOR = 0.85;

void init_pr_values_propagation(Digraph*);
void pr_compute_single_vertex_propagation(vertex_id_t, Digraph*);

#endif //KAPPA_PAGERANK_H
