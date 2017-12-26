#ifndef KAPPA_BUILD_GRAPH_STATIC_H
#define KAPPA_BUILD_GRAPH_STATIC_H

#include "graph.h"
#include "digraph.h"

Graph edge_array_to_graph(raw_edge_array&);
Digraph edge_array_to_digraph(raw_edge_array&);

#endif //KAPPA_BUILD_GRAPH_STATIC_H
