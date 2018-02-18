#include <ctime>
#include <fstream>

#include "digraph.h"
#include "read_tsv_disk.h"

// This should be just a hacky implementation that
// does additions of edges read from a given tsv file.
// A proper implementation should have separated tsv/stream ingestion and graph update parts.


void naive_incremental_compute_tsv(void(*compute)(Digraph*),
                                   Digraph *g,
                                   raw_edge_array &updates,
                                   const std::vector<unsigned long> &chunks_start_lines,
                                   unsigned long total_lines) {

    std::cout << "STARTING NAIVE INCREMENTAL COMPUTATION EXPERIMENT..." << std::endl;

    std::ofstream fs;

    fs.open("measurements.csv");
    fs << "Order" << " , " << "Size" << " , " << "Ingestion Rate" << " , " << "Ingestion CPU Time" << " , " << "Computation CPU time" << std::endl;

    unsigned long num_of_chunks = chunks_start_lines.size();

    unsigned long start_line = 0;
    unsigned long end_line = 0;

    for (int i = 0; i < num_of_chunks; i++) {
        start_line = chunks_start_lines[i];
        if (i < num_of_chunks - 1) {
            end_line = chunks_start_lines[i + 1];
        } else {
            end_line = total_lines;
            // TODO: FIX: In the last iteration we don't process the line which is equal to value of total_lines variable
        }

        std::cout << "Reading updates from chunk " << i+1 << std::endl;

        // Update the graph
        clock_t cpu_begin_update = clock();

        for(unsigned long j = start_line; j<end_line; j++) {
            g->add_edge(updates[j][0], updates[j][1]);
        }

        clock_t cpu_end_update = clock();
        double cpu_time_update = double(cpu_end_update - cpu_begin_update) / CLOCKS_PER_SEC;

        double ingestion_rate = (end_line-start_line) / cpu_time_update;

        std::cout << "Finished updating from chunk " << i+1 << std::endl;
        std::cout << "UPDATE TIME FOR chunk " << std::fixed << i+1 << ": " << cpu_time_update << std::endl;

        unsigned int order = g->get_order();
        unsigned int size = g->get_size();
        std::cout << "Order: " <<  order << std::endl;
        std::cout << "Size: " << size << std::endl;

        std::cout << "Executing computations..." << std::endl;

        // Execute compute function
        clock_t cpu_begin_compute = clock();

        compute(g);

        clock_t cpu_end_compute = clock();
        double cpu_time_compute = double(cpu_end_compute - cpu_begin_compute) / CLOCKS_PER_SEC;

        std::cout << "Finished computations." << std::endl;
        std::cout << "COMPUTE TIME AFTER chunk " << std::fixed << i+1 << ": " << cpu_time_compute << std::endl;

        fs << order << " , " << size << " , " << std::fixed << ingestion_rate << " , " << std::fixed << cpu_time_update << " , " << std::fixed << cpu_time_compute << std::endl;
    }

    fs.close();
}