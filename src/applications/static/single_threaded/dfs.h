#ifndef KAPPA_DFS_H
#define KAPPA_DFS_H

#include "graph.h"
#include "digraph.h"

void dfs(Graph*, unsigned int);
void dfs_compute(Graph*, unsigned int);

void dfs(Digraph*, unsigned int);
void dfs_compute(Digraph*, unsigned int);

#endif //KAPPA_DFS_H
