#include <deletions.h>
#include <gtest/gtest.h>
#include <insertions.h>
#include <runtimes/runtime_hard_barrier.h>
#include <iostream>
#include <read_from_disk/edgelist_to_graph.h>

#include "../data/datasets_constants.h"
#include "applications/dynamic/pagerank.h"
#include "applications/dynamic/sssp.h"
#include "applications/dynamic/wcc.h"
#include "compute.h"
#include "edge_array_to_graph.h"
#include "experiments/utils/dataset_split.h"
#include "global_scheduler.h"
#include "scheduler.h"

int main(int argc, char* argv[]) {
  // Parse command-line arguments
  if (argc != 4) {
    std::cerr << "Usage: kappa <no_of_cores> <additions_batch_size> "
                 "<deletions_batch_size>"
              << std::endl;

    exit(-1);
  }

  std::cout << "\n[START]\t\tSTARTING KAPPA..." << std::endl;

  uint no_of_cores = std::stoi(argv[1]);
  graph_size_t additions_batch_size = std::stoi(argv[2]);
  graph_size_t deletions_batch_size = std::stoi(argv[3]);

  // Initialise scheduler
  Scheduler& scheduler = GlobalScheduler::get_scheduler();

  GlobalScheduler::get_scheduler().halt_workers();
  scheduler.halt_workers();

  if (no_of_cores <= 16) {
    scheduler.init_numa_node(0, no_of_cores);
  } else if (no_of_cores <= 32) {
    scheduler.init_numa_node(0);
    scheduler.init_numa_node(1, no_of_cores - 16);
  } else if (no_of_cores <= 48) {
    scheduler.init_numa_node(0);
    scheduler.init_numa_node(1);
    scheduler.init_numa_node(2, no_of_cores - 32);
  } else if (no_of_cores <= 64) {
    scheduler.init_numa_node(0);
    scheduler.init_numa_node(1);
    scheduler.init_numa_node(2);
    scheduler.init_numa_node(3, no_of_cores - 48);
  } else {
    std::cerr << "no_of_cores not valid" << std::endl;

    exit(-1);
  }

  // ------------------------------------------------------------------------------------------

  // Choose core dataset file
  // std::string core_dataset_file = "/home/dm1515/data/test/zachary.edgelist";
  std::string core_dataset_file =
      "/home/dm1515/data/twitter_no_new_node/core_graph.snap";

  // Choose additons file
  // std::string additions_file =
  //    "/home/dm1515/data/test/zachary-additions.edgelist";
  std::string additions_file =
      "/home/dm1515/data/twitter_no_new_node/batch_1m_add.snap";

  // Choose deletions file
  // std::string deletions_file =
  //    "/home/dm1515/data/test/zachary-deletions.edgelist";
  std::string deletions_file =
      "/home/dm1515/data/twitter_no_new_node/batch_1m_del.snap";

  // Choose file with precomputed states for core graph
  // std::string core_states =
  // "/home/dm1515/data/core-graphs/precomputed-states/nx-higgs-shuff-core-10k-pr.txt";

  // ------------------------------------------------------------------------------------------

  // graph_size_t total_dataset_size = 83;
  graph_size_t total_dataset_size = twitter2010_size;
  std::cout << "[INFO]\t\tTotal number of edges in the dataset:\t\t\t\t\t"
            << total_dataset_size << std::endl;

  // graph_size_t total_dataset_order = 34;
  graph_size_t total_dataset_order = twitter2010_order;
  std::cout << "[INFO]\t\tTotal number of vertices in the dataset:\t\t\t\t"
            << total_dataset_order << std::endl;

  // graph_size_t core_size = 78;
  graph_size_t core_size = twitter2010_core_size;
  std::cout << "[INFO]\t\tNumber of edges in the core dataset:\t\t\t\t\t"
            << core_size << std::endl;

  // graph_size_t additions_size = edgelist_count_lines(additions_file);
  graph_size_t additions_size = 1000000; // TODO: Do not hardcode this value
  std::cout << "[INFO]\t\tTotal number of edges in additions file:\t\t\t\t"
            << additions_size << std::endl;

  // graph_size_t deletions_size = edgelist_count_lines(deletions_file);
  graph_size_t deletions_size = 1000000;
  std::cout << "[INFO]\t\tTotal number of edges in deletions file:\t\t\t\t"
            << deletions_size << std::endl;

  if (additions_batch_size > additions_size ||
      deletions_batch_size > deletions_size) {
    std::cerr << "Batch size must be smaller or equal to the total number of "
                 "edges in updates file!"
              << std::endl;
    exit(-1);
  }

  // ------------------------------------------------------------------------------------------

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

  // ------------------------------------------------------------------------------------------

  // Create a graph object
  // Digraph* g = Digraph(total_dataset_order, batch_size, updating,
  // computation_pr);
  Digraph g(total_dataset_order, additions_batch_size, updating,
            computation_pr);
  // TODO: Remove batch_size argument from Digraph constructor

  // Populate the graph with core set of edges
  edgelist_to_digraph(&g, core_dataset_file, core_size);

  std::cout << "[INFO]\t\tNumber of vertices after pre-populating:\t\t\t"
            << g.get_order() << std::endl;
  std::cout << "[INFO]\t\tNumber of edges after pre-populating:\t\t\t\t"
            << g.get_size() << std::endl;

  // Load the precomputed states for vertices in the core graph
  // preload_states(&g, core_states, ' ', 1, core_size);
  // dump_vertex_states(&g, "vertex-states-dump.txt");

  // ------------------------------------------------------------------------------------------

  raw_edge_array_t additions_edge_array;
  additions_edge_array = edgelist_to_edge_array(additions_file, additions_size);

  raw_edge_array_t deletions_edge_array;
  deletions_edge_array = edgelist_to_edge_array(deletions_file, deletions_size);

  // ------------------------------------------------------------------------------------------

  // Logically split addition and deletion edge arrays into batches
  graph_size_t beginning_additons = 1;
  graph_size_t end_additions = additions_size;

  graph_size_t beginning_deletions = 1;
  graph_size_t end_deletions = deletions_size;

  if (additions_batch_size > end_additions - beginning_additons + 1 ||
      deletions_batch_size > end_deletions - beginning_deletions + 1) {
    std::cerr << "Batch size must be smaller or equal than total number of "
                 "edges to be modified!"
              << std::endl;
    exit(-1);
  }

  std::vector<graph_size_t> split_additions = dataset_to_batches(
      beginning_additons, end_additions, additions_size, additions_batch_size);

  std::vector<graph_size_t> split_deletions = dataset_to_batches(
      beginning_deletions, end_deletions, deletions_size, deletions_batch_size);

  // ------------------------------------------------------------------------------------------

  // Main experiment part
  // Interchanging processes:
  // - updating the graph
  // - computing CC
  // - running user-defined computation

  run(computation_pr, updating, &g, additions_edge_array, deletions_edge_array,
      split_additions, split_deletions);

  // dump_vertex_states(&g, "vertex-states-dump.txt");

  // ------------------------------------------------------------------------------------------

  std::cout << "\n-------------------------------------------------------------"
               "----------------------------"
               "\n[END]\t\tKappa finished."
            << std::endl;

  return 0;
}
