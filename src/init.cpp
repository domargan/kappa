#include <iostream>
#include <list>
#include <functional>
#include <stack>
#include <utils/dump_vertex_states.h>

#include "digraph.h"
#include "naive_incremental_compute_tsv.h"
#include "dataset_split.h"
#include "pagerank.h"

#include "dataset_split_hardcoded.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    //std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    //std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";

    uint32_t lines = number_of_lines(dataset);

    //uint32_t core_size = 3000000; // Core graph is built from 3M updates
    //uint32_t chunks_size = 1000000; // Updates are processed in chunks of 1M

    //std::vector<uint32_t> split = dataset_split_by_size(lines, core_size, chunks_size);

    raw_edge_array_t edges;
    edges = tsv_to_edges(dataset, ' ');

    unsigned int max_vertex_num = unique_vertex_count(edges);

    state_t init_state = 1.0 / max_vertex_num; // init state for PageRank
    Digraph g = Digraph(max_vertex_num, init_state);

    naive_incremental_compute_tsv(pr_compute,
                                  &g,
                                  edges,
                                  SAMPLE_SPLIT_SINGLE_FINER,
                                  lines);

    //dump_vertex_states(&g, "results.txt");

    std::cout << "Kappa finished." << std::endl;

    return 0;
}