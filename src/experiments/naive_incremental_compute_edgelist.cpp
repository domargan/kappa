#include <ctime>
#include <fstream>

#include "compute.h"
#include "digraph.h"
#include "global_thread_pool.h"
#include "read_from_disk/edgelist_to_edge_array.h"
#include "update.h"

// This should be just a hacky implementation that
// does additions of edges read from a given edge_array.
// A proper implementation should have separated stream ingestion and graph update parts.

void naive_incremental_compute_edge_array(Computation computation,
                                   Digraph *g,
                                   raw_edge_array_t &updates,
                                   const std::vector<graph_size_t> &chunks_start_lines) {
    std::cout << "STARTING NAIVE INCREMENTAL COMPUTATION EXPERIMENT..." << std::endl;

    std::ofstream fs;
    fs.open("measurements.csv");
    fs << "Order" << " , " << "Size" << " , " << "Ingestion Rate" << " , " << "Ingestion CPU Time" << " , " << "Computation CPU time" << std::endl;

    graph_size_t num_of_chunks = chunks_start_lines.size() - 1; //-1 because the last element is just there to mark the end point

    graph_size_t start_line = 0;
    graph_size_t end_line = 0;

    for (int i = 0; i < num_of_chunks; ++i) {
        start_line = chunks_start_lines[i];
        end_line = chunks_start_lines[i+1];

        std::cout << "Reading updates from chunk " << i+1 << std::endl;

        // Update the graph
        std::chrono::system_clock::time_point system_begin_update = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_begin_update = std::chrono::steady_clock::now();

        std::vector<Update> updates_in_chunk;

        for (graph_size_t j = start_line - 1; j < end_line - 1; j++) {
            // std::cout << "READING EDGE " << updates[j][0] << " " << updates[j][1] << std::endl;

            vertex_id_t src = updates[j][0];
            vertex_id_t dst = updates[j][1];

            g->add_edge(src, dst);

            Update u(ADD, src, dst);
            updates_in_chunk.push_back(u);
        }

        std::chrono::system_clock::time_point system_end_update = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_end_update = std::chrono::steady_clock::now();

        float system_time_update = std::chrono::duration<float>(system_end_update - system_begin_update).count();
        float cpu_time_update = std::chrono::duration<float>(cpu_end_update - cpu_begin_update).count();

        float ingestion_rate = (end_line-start_line) / cpu_time_update;

        std::cout << "Finished updating from chunk " << i+1 << std::endl;
        std::cout << "(SYS) UPDATE TIME FOR chunk " << std::fixed << i+1 << ": " << system_time_update << std::endl;
        std::cout << "(CPU) UPDATE TIME FOR chunk " << std::fixed << i+1 << ": " << cpu_time_update << std::endl;

        // g->count_order();
        graph_size_t order = g->get_order();
        graph_size_t size = g->get_size();
        std::cout << "Order: " <<  order << std::endl;
        std::cout << "Size: " << size << std::endl;

        std::cout << "Executing computations for " << updates_in_chunk.size() << " updates" << std::endl;

        // Execute user-defined compute function
        std::chrono::system_clock::time_point system_begin_compute = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_begin_compute = std::chrono::steady_clock::now();

        for (auto &u : updates_in_chunk) {
            // TODO: Batch updates for same vertex
            if (u.type == ADD) {
                GlobalThreadPool::get_thread_pool().submit(COMPUTE, computation.on_add_edge, g, u.src, u.dst);
            } else {
                GlobalThreadPool::get_thread_pool().submit(COMPUTE, computation.on_remove_edge, g, u.src, u.dst);
            }
        }

        GlobalThreadPool::get_thread_pool().barrier();

        std::chrono::system_clock::time_point system_end_compute = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_end_compute = std::chrono::steady_clock::now();

        float system_time_compute = std::chrono::duration<float>(system_end_compute - system_begin_compute).count();
        float cpu_time_compute = std::chrono::duration<float>(cpu_end_compute - cpu_begin_compute).count();

        std::cout << "Finished computations." << std::endl;
        std::cout << "(SYS) COMPUTE TIME AFTER chunk " << std::fixed << i+1 << ": " << system_time_compute << std::endl;
        std::cout << "(CPU) COMPUTE TIME AFTER chunk " << std::fixed << i+1 << ": " << cpu_time_compute << std::endl;
        std::cout << "--------------------------------------------------------------------------------" << std::endl;

        fs << order << " , " << size << " , " << std::fixed << ingestion_rate << " , " << std::fixed << cpu_time_update << " , " << std::fixed << cpu_time_compute << std::endl;
    }

    fs.close();
}
