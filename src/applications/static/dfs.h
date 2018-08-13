#ifndef KAPPA_DFS_H
#define KAPPA_DFS_H

#include "graph.h"
#include "digraph.h"

void dfs(Graph*, vertex_id_t);
void dfs_compute(Graph*, vertex_id_t);

void dfs(Digraph*, vertex_id_t);
void dfs_compute(Digraph*, vertex_id_t);

#endif //KAPPA_DFS_H
