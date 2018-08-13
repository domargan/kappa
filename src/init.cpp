#include <gtest/gtest.h>
#include <iostream>

#include "applications/dynamic/pagerank.h"
#include "compute.h"
#include "core/utils/threading.h"
#include "edge_array_to_graph.h"
#include "experiments/naive_incremental_compute_edgelist.h"
#include "experiments/utils/dataset_split.h"
#include "read_from_disk/edgelist_to_graph.h"
#include "thread_pool.hpp"
#include "utils/dump_vertex_states.h"

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    // TODO: Pin main thread to CPU 0
    // TODO: Spawn thread for scheduler and pin it to CPU 1

    // ------------------------------------------------------------------------------------------ //

    // Choose dataset
    // std::string dataset = "/home/dm1515/data/test_data.edgelist";
    // std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    // std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";
    // std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    // std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled-head1M";
    std::string dataset = "/home/leopold/Developer/graphs/zachary.edgelist";

    // ------------------------------------------------------------------------------------------ //

    // Preload the data in memory, in form of an edge array
    raw_edge_array_t edge_array;
    edge_array = edgelist_to_edge_array(dataset, ' ');

    // Some basic dataset statistics
    graph_size_t max_vertex_num = unique_vertex_count(edge_array);
    graph_size_t num_dataset_entries = edgelist_count_lines(dataset);
    std::cout << "Number of edges in the dataset: " << num_dataset_entries << std::endl;
    std::cout << "Number of vertices in the dataset: " << max_vertex_num << std::endl;

    // ------------------------------------------------------------------------------------------ //

    // Set batch sizes
    graph_size_t batch_size = 10;

    // TODO: Use user-defined functions
    Computation computation;
    computation.init = PageRank::init;
    computation.incr_compute = PageRank::incr_compute;

    // Create a graph object
    Digraph g = Digraph(max_vertex_num, batch_size, computation);

    // Prepopulate the graph with some edges from the edge array
    /*
    edge_array_to_digraph(&g, edge_array, 1, 5000000);
    g.reset_touched_src_verts(); // DON'T FORGET TO RESET TOUCHED VERTICES AFTER PRE-POPULATING

    std::cout << "Graph size after pre-populating: " << g.get_size() << std::endl;

    std::vector<graph_size_t> *touched_verts = g.get_touched_src_verts();
    std::cout << "Touched vertices queue size: " << touched_verts->size() << std::endl;
    std::cout << "Touched vertices queue capacity: " << touched_verts->capacity() << std::endl;
    */

    // ------------------------------------------------------------------------------------------ //

    // Set a range of entries (lines) from the dataset, for updates to be applied to the graph
    graph_size_t beginning = 0;
    graph_size_t end = 78;

    // Logically split the entires in the dataset by lines, splitting into batches
    std::vector<graph_size_t> split = dataset_to_batches(beginning, end, num_dataset_entries, batch_size);

    // Main experiment part
    // Interchanging processes:
    // - updating the graph
    // - running incremental localised computation
    naive_incremental_compute_edge_array(computation,
                                       &g,
                                       edge_array,
                                       split);

    dump_vertex_states(&g, "results.txt");

    // ------------------------------------------------------------------------------------------ //

    std::cout << "Kappa finished." << std::endl;

    return 0;
}
