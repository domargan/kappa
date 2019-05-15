#include "insertions.h"

namespace Insertions {
    void edge_insertion(Digraph *g, vertex_id_t src, vertex_id_t dst) {
        g->add_edge(src, dst);
    }
}