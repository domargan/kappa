#include <iostream>
#include <list>
#include <functional>
#include <stack>

#include "read_from_disk/edgelist_to_graph.h"
#include "naive_incremental_compute_edgelist.h"
#include "dataset_split.h"
#include "pagerank.h"
#include "compute.h"

#include <gtest/gtest.h>


int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    //std::string dataset = "/home/dm1515/data/test_data.edgelist";
    //std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    //std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";
    std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    //std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled-head1M";

    graph_size_t lines = number_of_lines(dataset);

    raw_edge_array_t edges;
    edges = edgelist_to_edges(dataset, ' ');

    unsigned int max_vertex_num = unique_vertex_count(edges);

    state_t init_state = 1.0 / max_vertex_num; // init state for PageRank

    graph_size_t core_size = 1000;
    graph_size_t chunks_size = core_size;

    graph_size_t beginning = 5000001;
    graph_size_t end = 5010000;

    Digraph g = edgelist_to_digraph(dataset, ' ', 1, 5000000, max_vertex_num, init_state, chunks_size); //Prepopulating the graph...
    std::cout << "Graph size after pre-populating: " << g.get_size() << std::endl;

    std::vector<graph_size_t> *touched_verts = g.get_touched_src_verts();
    std::cout << "Touched vertices queue size: " << touched_verts->size() << std::endl;
    std::cout << "Touched vertices queue capacity: " << touched_verts->capacity() << std::endl;

    //Computing PR values for the initial prepopulated graph (global)
    run_global(&g, pr_compute_single_vertex);

    std::vector<graph_size_t> split = dataset_to_batches(beginning, end, lines, core_size, chunks_size);

    //Updating and computing incremental PR (local)
    naive_incremental_compute_edgelist(pr_compute_single_vertex,
                                  &g,
                                  edges,
                                  split);

    //dump_vertex_states(&g, "results.txt");

    std::cout << "Kappa finished." << std::endl;
    return 0;
}
