#include <algorithm>
#include <set>
#include "read_from_disk/tsv_to_edge_array.h"
#include "edge_array_to_graph.h"

Graph edge_array_to_graph(raw_edge_array& edge_array, float init_state) {
    uint32_t v_num = unique_vertex_count(edge_array);

    Graph graph = Graph(v_num, init_state);

    for(auto edge : edge_array) {
        graph.add_edge(edge[0], edge[1]);
    }

    return graph;
}

Digraph edge_array_to_digraph(raw_edge_array& edge_array, float init_state) {
    uint32_t v_num = unique_vertex_count(edge_array);

    Digraph digraph = Digraph(v_num, init_state);

    for(auto edge : edge_array) {
        digraph.add_edge(edge[0], edge[1]);
    }

    return digraph;
}
