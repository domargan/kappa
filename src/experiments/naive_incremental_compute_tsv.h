#ifndef KAPPA_NAIVE_INCREMENTAL_COMPUTE_TSV_H
#define KAPPA_NAIVE_INCREMENTAL_COMPUTE_TSV_H

#include "digraph.h"
#include "read_tsv_disk.h"

void naive_incremental_compute_tsv(void(*)(Digraph*),
                                   Digraph*,
                                   raw_edge_array&,
                                   const std::vector<unsigned long> &,
                                   unsigned long);

#endif //KAPPA_NAIVE_INCREMENTAL_COMPUTE_TSV_H
