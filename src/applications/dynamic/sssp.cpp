#include <algorithm>
#include <iostream>
#include <limits>

#include "sssp.h"

namespace SSSP {
state_t get_min_distance(Digraph* g, vertex_id_t v) {
  state_t min = std::numeric_limits<double>::infinity();

  for (auto neighbour : g->get_in_neighborhood(v)) {
    state_t neighbour_state = g->get_state(neighbour);

    if (neighbour_state < min) {
      min = neighbour_state;
    }
  }

  return min;
}

void init_state(Digraph* g, vertex_id_t v) {
  state_t state = (v == SOURCE) ? 0 : get_min_distance(g, v) + 1;

  g->set_state(v, state);
}

void on_activate(Digraph* g, vertex_id_t v) {
  state_t old_distance = g->get_state(v);
  state_t min = get_min_distance(g, v);

  if (old_distance > min + 1) {
    g->set_state(v, min + 1);

    for (auto neighbour : g->get_out_neighborhood(v)) {
      g->activate_vertex(neighbour);
    }
  }
}

void on_add_edge(Digraph* g, vertex_id_t src, vertex_id_t dst) {
  state_t src_state = g->get_state(src);
  state_t dst_state = g->get_state(dst);

  if (src_state + 1 < dst_state) {
    g->set_state(dst, src_state + 1);

    for (auto neighbour : g->get_out_neighborhood(dst)) {
      g->activate_vertex(neighbour);
    }
  }
}

void on_remove_edge(Digraph* g, vertex_id_t src, vertex_id_t dst) {
  if (g->get_state(src) + 1 == g->get_state(dst)) {
    g->activate_vertex(dst);
  }
}
}  // namespace SSSP
