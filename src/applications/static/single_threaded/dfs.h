#ifndef KAPPA_DFS_H
#define KAPPA_DFS_H

#include "graph.h"
#include "digraph.h"

void dfs(Graph*, uint32_t);
void dfs_compute(Graph*, uint32_t);

void dfs(Digraph*, uint32_t);
void dfs_compute(Digraph*, uint32_t);

#endif //KAPPA_DFS_H
