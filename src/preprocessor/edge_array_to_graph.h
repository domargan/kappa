#ifndef KAPPA_EDGE_ARRAY_TO_GRAPH_H
#define KAPPA_EDGE_ARRAY_TO_GRAPH_H

#include "digraph.h"
#include "read_from_disk/edgelist_to_edge_array.h"

void edge_array_to_digraph(Digraph*,
                           raw_edge_array_t&,
                           graph_size_t,
                           graph_size_t);

#endif  // KAPPA_EDGE_ARRAY_TO_GRAPH_H
