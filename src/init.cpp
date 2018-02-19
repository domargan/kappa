#include <iostream>
#include <list>
#include <functional>
#include <stack>

#include "digraph.h"
#include "naive_incremental_compute_tsv.h"
#include "read_tsv_disk.h"
#include "build_graph_static.h"
#include "dataset_split.h"
#include "pagerank.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";

    unsigned long lines = number_of_lines(dataset);

    unsigned long core_size = (lines * 30) / 100; // Core graph is 30% of the entire dataset
    unsigned long chunks_size = (lines * 10) / 100; // Updates are processed in chunks of 10% of the dataset

    std::vector<unsigned long> split = dataset_split_by_size(lines, core_size, chunks_size);

    raw_edge_array edges;
    edges = tsv_to_edges(dataset, ' ');

    unsigned int max_vertex_num = unique_vertex_count(edges);

    double init_state = 1.0 / max_vertex_num; // init state for PageRank
    Digraph g = Digraph(max_vertex_num, init_state);

    naive_incremental_compute_tsv(pr_compute,
                                  &g,
                                  edges,
                                  split,
                                  lines);

    std::cout << "Kappa finished." << std::endl;

    return 0;
}