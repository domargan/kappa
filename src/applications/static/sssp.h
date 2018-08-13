#ifndef KAPPA_SSSP_H
#define KAPPA_SSSP_H

#include <list>
#include <vector>

#include "digraph.h"

std::vector<int> bfs_previously_visited(Digraph*, int);
std::list<int> shortest_path(Digraph*, std::vector<int>, int, int);
std::vector<std::list<int>> sssp_compute(Digraph*, int);

#endif //KAPPA_SSSP_H
