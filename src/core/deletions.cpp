#include "deletions.h"

namespace Deletions {
void edge_deletion(Digraph* g, vertex_id_t src, vertex_id_t dst) {
  g->remove_edge(src, dst);

  // TODO: implement src and dst reachability analysis here
}
}