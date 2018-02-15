#include <algorithm>
#include <set>
#include "read_tsv_disk.h"
#include "build_graph_static.h"

Graph edge_array_to_graph(raw_edge_array& edge_array) {
    unsigned int v_num = unique_vertex_count(extract_vertices(edge_array));

    Graph graph = Graph(v_num);

    for(auto edge : edge_array) {
        graph.add_edge(edge[0], edge[1]);
    }

    return graph;
}

Digraph edge_array_to_digraph(raw_edge_array& edge_array) {
    unsigned int v_num = unique_vertex_count(extract_vertices(edge_array));

    Digraph digraph = Digraph(v_num);

    for(auto edge : edge_array) {
        digraph.add_edge(edge[0], edge[1]);
    }

    return digraph;
}
