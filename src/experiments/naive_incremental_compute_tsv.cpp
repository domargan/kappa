#include <ctime>
#include <fstream>

#include "compute.h"
#include "read_from_disk/tsv_to_edge_array.h"

// This should be just a hacky implementation that
// does additions of edges read from a given tsv file.
// A proper implementation should have separated tsv/stream ingestion and graph update parts.

void naive_incremental_compute_tsv(void(*compute)(uint32_t, Digraph*),
                                   Digraph *g,
                                   raw_edge_array_t &updates,
                                   const std::vector<uint32_t> &chunks_start_lines) {

    std::cout << "STARTING NAIVE INCREMENTAL COMPUTATION EXPERIMENT..." << std::endl;

    g->set_state_change_tolerance(DEFAULT_CHANGE_TOLERANCE);

    std::ofstream fs;

    fs.open("measurements.csv");
    fs << "Order" << " , " << "Size" << " , " << "Ingestion Rate" << " , " << "Ingestion CPU Time" << " , " << "Computation CPU time" << std::endl;

    uint32_t num_of_chunks = chunks_start_lines.size() - 1; //-1 because the last element is just there to mark the end point

    uint32_t start_line = 0;
    uint32_t end_line = 0;

    for (int i = 0; i < num_of_chunks; i++) {
        start_line = chunks_start_lines[i];
        end_line = chunks_start_lines[i+1];

        //std::cout << "Reading updates from chunk " << i+1 << std::endl;

        // Update the graph
        clock_t cpu_begin_update = clock();

        for (uint32_t j = start_line - 1; j < end_line - 1; j++) {
            g->add_edge(updates[j][0], updates[j][1]);
            //std::cout << "READING EDGE " << updates[j][0] << " " << updates[j][1] << std::endl;
        }

        clock_t cpu_end_update = clock();
        float cpu_time_update = float(cpu_end_update - cpu_begin_update) / CLOCKS_PER_SEC;

        float ingestion_rate = (end_line-start_line) / cpu_time_update;

        //std::cout << "Finished updating from chunk " << i+1 << std::endl;
        //std::cout << "UPDATE TIME FOR chunk " << std::fixed << i+1 << ": " << cpu_time_update << std::endl;

        //g->count_order();
        uint32_t order = g->get_order();
        uint32_t size = g->get_size();
        std::cout << "Order: " <<  order << std::endl;
        std::cout << "Size: " << size << std::endl;

        //boost::circular_buffer<uint32_t> *touched_verts = g->get_touched_src_verts();
        std::vector<uint32_t> *touched_verts = g->get_touched_src_verts();

        std::cout << "Touched vertices queue size: " << touched_verts->size() << std::endl;
        std::cout << "Touched vertices queue capacity: " << touched_verts->capacity() << std::endl;

        //std::cout << "Executing computations..." << std::endl;

        // Execute user-defined compute function
        clock_t cpu_begin_compute = clock();

        //run(g, compute);
        run_local(g, compute);

        clock_t cpu_end_compute = clock();
        float cpu_time_compute = float(cpu_end_compute - cpu_begin_compute) / CLOCKS_PER_SEC;

        //std::cout << "Finished computations." << std::endl;
        //std::cout << "COMPUTE TIME AFTER chunk " << std::fixed << i+1 << ": " << cpu_time_compute << std::endl;

        fs << order << " , " << size << " , " << std::fixed << ingestion_rate << " , " << std::fixed << cpu_time_update << " , " << std::fixed << cpu_time_compute << std::endl;
    }

    fs.close();
}
