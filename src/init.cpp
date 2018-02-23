#include <iostream>
#include <list>
#include <functional>
#include <stack>
#include <utils/dump_vertex_states.h>

#include "digraph.h"
#include "naive_incremental_compute_tsv.h"
#include "dataset_split.h"
#include "pagerank.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    //std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";

    uint32_t lines = number_of_lines(dataset);

    //uint32_t core_size = (lines * 30) / 100; // Core graph is 30% of the entire dataset
    //uint32_t chunks_size = (lines * 10) / 100; // Updates are processed in chunks of 10% of the dataset

//    uint32_t core_size = 3000000; // Core graph is built from 3M updates
//    uint32_t chunks_size = 1000000; // Updates are processed in chunks of 1M

    uint32_t core_size = 1000000; // Core graph is built from 3M updates
    uint32_t chunks_size = 1000000; // Updates are processed in chunks of 1M

    std::vector<uint32_t> split = dataset_split_by_size(lines, core_size, chunks_size);

/*
 * //Single update granularity
    std::vector<uint32_t> split{0,
                                999999,
                                1000000,1000001,
                                1999999,
                                2000000, 2000001,
                                3999999,
                                4000000, 4000001,
                                5999999,
                                6000000, 6000001,
                                7999999,
                                8000000, 8000001,
                                9999999,
                                10000000, 10000001,
                                11999999,
                                12000000, 12000001,
                                13999999,
                                14000000, 14000001};
*/
    raw_edge_array edges;
    edges = tsv_to_edges(dataset, ' ');

    unsigned int max_vertex_num = unique_vertex_count(edges);

    double init_state = 1.0 / max_vertex_num; // init state for PageRank
    //float init_state = 1;
    Digraph g = Digraph(max_vertex_num, init_state);

    naive_incremental_compute_tsv(pr_compute,
                                  &g,
                                  edges,
                                  split,
                                  lines);

    dump_vertex_states(&g, "results.txt");

    std::cout << "Kappa finished." << std::endl;

    return 0;
}