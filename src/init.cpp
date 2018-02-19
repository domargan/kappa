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

    std::string dataset = "/home/domargan/test.edgelist";
    unsigned long core_size = 20;
    unsigned long chunks_size = 5;

    unsigned long lines = number_of_lines(dataset);

    std::vector<unsigned long> split = dataset_split_by_size(lines, core_size, chunks_size);

    raw_edge_array edges;
    edges = tsv_to_edges(dataset, ' ');

    unsigned int vertex_num = unique_vertex_count(extract_vertices(edges));
    Digraph g = Digraph(vertex_num);

    naive_incremental_compute_tsv(pr_compute,
                                  &g,
                                  edges,
                                  split,
                                  lines);

    std::cout << "Kappa finished." << std::endl;

    return 0;
}