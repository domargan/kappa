#ifndef KAPPA_NAIVE_INCREMENTAL_COMPUTE_edgelist_H
#define KAPPA_NAIVE_INCREMENTAL_COMPUTE_edgelist_H

#include "digraph.h"
#include "read_from_disk/edgelist_to_edge_array.h"

void naive_incremental_compute_edgelist(void(*)(vertex_id_t, Digraph*),
                                   Digraph*,
                                   raw_edge_array_t&,
                                   const std::vector<vertex_id_t> &);

#endif //KAPPA_NAIVE_INCREMENTAL_COMPUTE_edgelist_H
