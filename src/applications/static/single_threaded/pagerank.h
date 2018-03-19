#ifndef KAPPA_PAGERANK_H
#define KAPPA_PAGERANK_H

const float DEFAULT_DAMPING_FACTOR = 0.85;

void init_pr_values(Digraph*);
void pr_compute_single_vertex(uint32_t, Digraph*);

#endif //KAPPA_PAGERANK_H
