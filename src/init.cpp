#include <iostream>
#include <list>
#include <digraph.h>
#include <graph.h>
#include <stack>
#include "read_tsv_disk.h"
#include "pagerank.h"
#include "dfs.h"
#include "bfs.h"
#include "sssp.h"
#include "connected_components.h"
#include "build_graph_static.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    raw_edge_array edges;
    edges = tsv_to_edges("/home/domargan/Workspace/Code/C++/kappa/data/twitter/higgs-social_network.edgelist", ' ');
    Graph g = edge_array_to_graph(edges);

    dfs_compute(&g,2);

    /*

    Graph g = Graph(11);

    // Graf s prezentacije sa Yorka
    g.add_edge(0,8);
    g.add_edge(1,3);
    g.add_edge(1,7);
    g.add_edge(1,9);
    g.add_edge(1,2);
    g.add_edge(2,8);
    g.add_edge(2,1);
    g.add_edge(2,4);
    g.add_edge(3,4);
    g.add_edge(3,5);
    g.add_edge(3,1);
    g.add_edge(4,2);
    g.add_edge(4,3);
    g.add_edge(5,3);
    g.add_edge(5,6);
    g.add_edge(6,7);
    g.add_edge(6,5);
    g.add_edge(7,1);
    g.add_edge(7,6);
    g.add_edge(8,2);
    g.add_edge(8,0);
    g.add_edge(8,9);
    g.add_edge(9,1);
    g.add_edge(9,8);
    g.add_edge(10,11);

    */

    std::cout << "Order: " << g.get_order() << std::endl;
    std::cout << "Size: " << g.get_size() << std::endl;

    std::cout << "Kappa finished." << std::endl;
    return 0;
}