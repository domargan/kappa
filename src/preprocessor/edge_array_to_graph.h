#ifndef KAPPA_EDGE_ARRAY_TO_GRAPH_H
#define KAPPA_EDGE_ARRAY_TO_GRAPH_H

#include "graph.h"
#include "digraph.h"

Graph edge_array_to_graph(raw_edge_array&, double);
Digraph edge_array_to_digraph(raw_edge_array&, double);

#endif //KAPPA_EDGE_ARRAY_TO_GRAPH_H
