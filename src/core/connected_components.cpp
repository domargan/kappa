#include <iostream>
#include <stack>
#include "connected_components.h"

cc_map_t cc_map = {};

void dfs_explore(Digraph *g, vertex_id_t v) {
    std::stack<vertex_id_t> stack;

    stack.push(v);

    while (!stack.empty()) {
        v = stack.top();
        stack.pop();

        if (!g->has_been_visited(v)) {
            //std::cout << v << std::endl;
            g->set_visited(v);
            g->set_component_label(v, g->get_cc_count());
            cc_map[g->get_cc_count()].push_back(v);
        }

        for (auto neighbor : *(g->get_in_out_neighborhood(v))) {
            if (!g->has_been_visited(neighbor)) {
                stack.push(neighbor);
            }
        }
    }
}

void dfs_explore_recursive(Digraph *g, vertex_id_t v) {
    g->set_visited(v);

    g->set_component_label(v, g->get_cc_count());
    cc_map[g->get_cc_count()].push_back(v);

    for (auto neighbor : *(g->get_in_out_neighborhood(v))) {
        if (!g->has_been_visited(neighbor)) {
            dfs_explore_recursive(g, neighbor);
        }
    }
}

void set_components_labels(Digraph *g) {
    g->reset_visited_verts();
    g->reset_cc_count_and_labels();
    cc_map.clear();

    graph_size_t max_order = g->get_max_order();
    for (graph_size_t v=0; v<max_order; v++) {
        if (g->has_vertex(v)) {
            if (!g->has_been_visited(v)) {
                dfs_explore_recursive(g, v);

                g->increment_cc_count();
            }
        }
    }
}
