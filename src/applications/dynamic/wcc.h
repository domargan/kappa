#ifndef KAPPA_WCC_H
#define KAPPA_WCC_H

#include "digraph.h"

namespace WCC {
void init_state(Digraph*, vertex_id_t);
void on_activate(Digraph*, vertex_id_t);
void on_add_edge(Digraph*, vertex_id_t, vertex_id_t);
void on_remove_edge(Digraph*, vertex_id_t, vertex_id_t);
}  // namespace WCC

#endif  // KAPPA_WCC_H
