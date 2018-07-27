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
#include <edge_array_to_graph.h>
#include <thread_pool.hpp>

int main() {
    std::cout << "Launching Kappa...\n" << std::endl;

    //std::string dataset = "/home/dm1515/data/test_data.edgelist";
    //std::string dataset = "/home/dm1515/data/twitter-2010-shuffled.txt";
    std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";
    //std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    //std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled-head1M";

    // std::string dataset = "/home/leopold/Developer/CLionProjects/kappa/data/twitter/higgs-social_network.edgelist";

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
    graph_size_t batch_size = 1000;

    // Set init state of vertices
    state_t init_state = 1.0 / max_vertex_num; // init state for PageRank

    // Create a graph object
    Digraph g = Digraph(max_vertex_num, init_state, batch_size);

    // Prepopulate the graph with some edges from the edge array
    edge_array_to_digraph(&g, edge_array, 1, 5000000);
    g.reset_touched_src_verts(); // DON'T FORGET TO RESET TOUCHED VERTICES AFTER PRE-POPULATING

    std::cout << "Graph size after pre-populating: " << g.get_size() << std::endl;

    std::vector<graph_size_t> *touched_verts = g.get_touched_src_verts();
    std::cout << "Touched vertices queue size: " << touched_verts->size() << std::endl;
    std::cout << "Touched vertices queue capacity: " << touched_verts->capacity() << std::endl;

    // ------------------------------------------------------------------------------------------ //

    // Run computations for the initial prepopulated graph (global computation)
    run_global(&g, pr_compute_single_vertex);

    // Set a range of entries (lines) from the dataset, for updates to be applied to the graph
    graph_size_t beginning = 5000001;
    graph_size_t end = 5010000;

    // Logically split the entires in the dataset by lines, splitting into batches
    std::vector<graph_size_t> split = dataset_to_batches(beginning, end, num_dataset_entries, batch_size);


    // Main experiment part
    // Interchanging processes:
    // - updating the graph
    // - running incremental localised computation
    naive_incremental_compute_edge_array(pr_compute_single_vertex,
                                       &g,
                                       edge_array,
                                       split);

    //dump_vertex_states(&g, "results.txt");

    std::cout << "Kappa finished." << std::endl;

    return 0;
}
