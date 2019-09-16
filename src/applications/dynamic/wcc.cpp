#include <atomic>

#include "wcc.h"

namespace WCC {
uint get_new_label() {
  static std::atomic<uint> counter{0};

  return ++counter;
}

state_t get_max_label(Digraph* g, vertex_id_t v) {
  state_t max = 0;

  for (auto neighbour : g->get_in_out_neighborhood(v)) {
    state_t neighbour_state = g->get_state(neighbour);

    if (neighbour_state > max) {
      max = neighbour_state;
    }
  }

  return max;
}

void init_state(Digraph* g, vertex_id_t v) {
  g->set_state(v, get_max_label(g, v));
}

void on_activate(Digraph* g, vertex_id_t v) {
  state_t old_label = g->get_state(v);

  state_t max_label = get_max_label(g, v);
  g->set_state(v, max_label);

  if (old_label != max_label) {
    for (auto neighbour : g->get_in_out_neighborhood(v)) {
      g->activate_vertex(neighbour);
    }
  }
}

void on_add_edge(Digraph* g, vertex_id_t src, vertex_id_t dst) {
  state_t src_state = g->get_state(src);
  state_t dst_state = g->get_state(dst);

  if (src_state != dst_state) {
    vertex_id_t smaller = (src_state < dst_state) ? src : dst;
    state_t new_max = (src_state < dst_state) ? dst_state : src_state;

    g->set_state(smaller, new_max);

    for (auto neighbour : g->get_in_out_neighborhood(smaller)) {
      g->activate_vertex(neighbour);
    }
  }
}

void on_remove_edge(Digraph* g, vertex_id_t src, vertex_id_t dst) {
  g->set_state(src, get_new_label());

  for (auto neighbour : g->get_in_out_neighborhood(src)) {
    g->activate_vertex(neighbour);
  }

  g->set_state(dst, get_new_label());

  for (auto neighbour : g->get_in_out_neighborhood(dst)) {
    g->activate_vertex(neighbour);
  }
}
}  // namespace WCC
