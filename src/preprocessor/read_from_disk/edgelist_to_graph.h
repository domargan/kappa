#ifndef KAPPA_EDGELIST_TO_GRAPH_H
#define KAPPA_EDGELIST_TO_GRAPH_H

#include "digraph.h"

void edgelist_to_digraph(Digraph*,
                         const std::string&,
                         char,
                         graph_size_t,
                         graph_size_t);

#endif  // KAPPA_EDGELIST_TO_GRAPH_H
