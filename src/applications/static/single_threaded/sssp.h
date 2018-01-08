#ifndef KAPPA_SSSP_H
#define KAPPA_SSSP_H

#include "graph.h"

std::vector<int> bfs_previously_visited(Graph*, int);
std::list<int> shortest_path(Graph*, std::vector<int>, int, int);
std::vector<std::list<int>> sssp_compute(Graph*, int);

#endif //KAPPA_SSSP_H
