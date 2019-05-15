#include <gtest/gtest.h>
#include <iostream>
#include <insertions.h>
#include <deletions.h>
#include <runtimes/runtime_hard_barrier.h>

#include "applications/dynamic/pagerank.h"
#include "applications/dynamic/sssp.h"
#include "applications/dynamic/wcc.h"
#include "compute.h"
#include "core/utils/threading.h"
#include "edge_array_to_graph.h"
#include "experiments/naive_incremental_compute_edgelist.h"
#include "experiments/utils/dataset_split.h"
#include "global_thread_pool.h"
#include "preload_states.h"
#include "read_from_disk/edgelist_to_graph.h"
#include "thread_pool.h"
#include "utils/dump_vertex_states.h"

int main(int argc, char *argv[]) {
    // Parse command-line arguments
    if (argc != 3) {
        std::cerr << "Usage: kappa <no_of_cores> <batch_size>" << std::endl;

        exit(-1);
    }

    uint no_of_cores = std::stoi(argv[1]);
    graph_size_t batch_size = std::stoi(argv[2]);

    // Initialise thread pool
    ThreadPool &threadPool = GlobalThreadPool::get_thread_pool();

    if (no_of_cores <= 16) {
        threadPool.init_numa_node(0, no_of_cores);
    } else if (no_of_cores <= 32) {
        threadPool.init_numa_node(0);
        threadPool.init_numa_node(1, no_of_cores - 16);
    } else if (no_of_cores <= 48) {
        threadPool.init_numa_node(0);
        threadPool.init_numa_node(1);
        threadPool.init_numa_node(2, no_of_cores - 32);
    } else if (no_of_cores <= 64) {
        threadPool.init_numa_node(0);
        threadPool.init_numa_node(1);
        threadPool.init_numa_node(2);
        threadPool.init_numa_node(3, no_of_cores - 48);
    } else {
        std::cerr << "no_of_cores not valid" << std::endl;

        exit(-1);
    }

    // ------------------------------------------------------------------------------------------ //

    // Choose dataset
    // std::string dataset = "/home/dm1515/data/zachary.edgelist";
    // std::string dataset = "/home/dm1515/data/twitter-2010.txt";
    // std::string dataset = "/home/dm1515/data/higgs-social_network.edgelist";
    // std::string dataset = "/home/dm1515/data/higgs-social_network-shuffled.edgelist";
    // std::string dataset = "/home/dm1515/data/sx-stackoverflow-no-stamps.txt";
    std::string dataset = "/home/dm1515/data/wikipedia_link_en/out.wikipedia_link_en_11k";
    //std::string dataset = "/home/dm1515/data/wikipedia_link_en/out.wikipedia_link_en";

    // Precomputed states for vertices in the core graph
    // std::string core_states = "/home/dm1515/kappa/utils/nx-validate/results/nx-sssp-zachary.txt";
    //std::string core_states = "/home/dm1515/data/core-graphs/precomputed-states/nx-higgs-shuff-core-10k-pr.txt";
    //std::string core_states = "/home/dm1515/data/core-graphs/precomputed-states/nx-higgs-shuff-core-14M-pr.txt";
    // std::string core_states = "/home/dm1515/data/core-graphs/precomputed-states/nx-higgs-shuff-core-14M-sssp.txt";
    // std::string core_states = "/home/dm1515/data/core-graphs/precomputed-states/nx-higgs-shuff-core-14M-wcc.txt";
    // std::string core_states = "/home/dm1515/data/core-graphs/precomputed-states/sx-stackoverflow-no-stamps-core-63M.txt";

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

    // Set core graph size (#edges)
    //graph_size_t core_size = 0;
    //graph_size_t core_size = 14000000;
    //graph_size_t core_size = 63000000;
    //graph_size_t core_size = 189071210;
    graph_size_t core_size = 10000;


    // TODO: Use user-defined functions

    Updating updating;
    updating.edge_insertion = Insertions::edge_insertion;
    updating.edge_deletion = Deletions::edge_deletion;

    Computation computation_pr;
    computation_pr.init_state = PageRank::init_state;
    computation_pr.on_activate = PageRank::on_activate;
    computation_pr.on_add_edge = PageRank::on_add_edge;
    computation_pr.on_remove_edge = PageRank::on_remove_edge;

    Computation computation_sssp;
    computation_sssp.init_state = SSSP::init_state;
    computation_sssp.on_activate = SSSP::on_activate;
    computation_sssp.on_add_edge = SSSP::on_add_edge;
    computation_sssp.on_remove_edge = SSSP::on_remove_edge;

    Computation computation_wcc;
    computation_wcc.init_state = WCC::init_state;
    computation_wcc.on_activate = WCC::on_activate;
    computation_wcc.on_add_edge = WCC::on_add_edge;
    computation_wcc.on_remove_edge = WCC::on_remove_edge;

    // Create a graph object
    Digraph g = Digraph(max_vertex_num, batch_size, updating, computation_pr);

    // Populate the graph with core set of edges
    edge_array_to_digraph(&g, edge_array, 1, core_size);
    std::cout << "Number of vertices after pre-populating: " << g.get_order() << std::endl;
    std::cout << "Number of edges after pre-populating: " << g.get_size() << std::endl;

    /*
    std::vector<graph_size_t> *touched_verts = g.get_touched_src_verts();
    std::cout << "Touched vertices queue size: " << touched_verts->size() << std::endl;
    std::cout << "Touched vertices queue capacity: " << touched_verts->capacity() << std::endl;
    */

    // Load the precomputed states for vertices in the core graph
    //preload_states(&g, core_states, ' ', 1, core_size);
    //dump_vertex_states(&g, "preloaded-states-check.txt");

    // ------------------------------------------------------------------------------------------ //

    // Set a range of entries (lines) from the dataset, for updates to be applied to the graph
    graph_size_t beginning = core_size + 1;
    //graph_size_t end = 10010000;
    //graph_size_t end = 14000000;
    //graph_size_t end = 14010000;
    //graph_size_t end = 14100000;
    //graph_size_t end = 14500000;
    //graph_size_t end = 378142420;
    //graph_size_t end = 189371211;
    graph_size_t end = 11000;

    // Logically split the entires in the dataset by lines, splitting into batches
    std::vector<graph_size_t> split = dataset_to_batches(beginning, end, num_dataset_entries, batch_size);

    // Main experiment part
    // Interchanging processes:
    // - updating the graph
    // - running incremental localised computation
    run(computation_pr,
            updating,
            &g,
            edge_array,
            split);

    //dump_vertex_states(&g, "results.txt");

    // ------------------------------------------------------------------------------------------ //

    std::cout << "Kappa finished." << std::endl;

    return 0;
}
