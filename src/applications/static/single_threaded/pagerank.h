#ifndef KAPPA_PAGERANK_H
#define KAPPA_PAGERANK_H

const float DEFAULT_DAMPING_FACTOR = 0.85;
const int DEFAULT_MAX_ITERATIONS = 2;

void init_pr_values(Digraph*);
void pr_compute(Digraph*);

#endif //KAPPA_PAGERANK_H
