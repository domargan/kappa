#include <iostream>
#include <list>
#include <functional>
#include <stack>
#include <utils/dump_vertex_states.h>

#include "naive_incremental_compute_tsv.h"
#include "dataset_split.h"
#include "pagerank.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    //std::string dataset = "/home/dm1515/data/test_data.edgelist";
    //std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    //std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";
    std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    //std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled-head1M";

    uint32_t lines = number_of_lines(dataset);

    uint32_t core_size = 1000000;
    uint32_t chunks_size = 1000000;

    std::vector<uint32_t> split = dataset_to_batches(1, lines, lines, core_size, chunks_size);

    raw_edge_array_t edges;
    edges = tsv_to_edges(dataset, ' ');

    unsigned int max_vertex_num = unique_vertex_count(edges);

    state_t init_state = 1.0 / max_vertex_num; // init state for PageRank
    Digraph g = Digraph(max_vertex_num, init_state);

    naive_incremental_compute_tsv(pr_compute,
                                  &g,
                                  edges,
                                  split);

    //dump_vertex_states(&g, "results.txt");

    std::cout << "Kappa finished." << std::endl;

    return 0;
}
