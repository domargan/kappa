#include "runtime_hard_barrier.h"

#include <ctime>
#include <fstream>

#include "compute.h"
#include "digraph.h"
#include "global_scheduler.h"
#include "read_from_disk/edgelist_to_edge_array.h"
#include "update.h"
#include "../connected_components.h"

// TODO: Proper ingestion component

void run(Computation computation,
        Updating updating,
        Digraph *g,
        raw_edge_array_t &updates,
        const std::vector<graph_size_t> &chunks_start_lines) {
    std::cout << "STARTING RUNTIME..." << std::endl;

    std::ofstream fs;
    fs.open("measurements.csv");
    fs << "Order" << " , " << "Size" << " , " << "Ingestion Rate" << " , " << "Ingestion CPU Time" << " , " << "Computation CPU time" << std::endl;

    std::ofstream fs2;
    //fs2.open("task_amount.txt");

    graph_size_t num_of_chunks = chunks_start_lines.size() - 1; //-1 because the last element is just there to mark the end point

    graph_size_t start_line = 0;
    graph_size_t end_line = 0;

    // Reset start time for queue evaluation
    GlobalScheduler::get_scheduler().tp_start = std::chrono::system_clock::now();

    // Measure end-to-end time
    std::chrono::system_clock::time_point system_begin_total = std::chrono::system_clock::now();
    std::chrono::steady_clock::time_point cpu_begin_total = std::chrono::steady_clock::now();

    for (graph_size_t i = 0; i < num_of_chunks; ++i) {
        start_line = chunks_start_lines[i];
        end_line = chunks_start_lines[i+1];

        std::cout << "Reading updates from chunk " << i+1 << std::endl;

        // Update the graph
        std::chrono::system_clock::time_point system_begin_update = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_begin_update = std::chrono::steady_clock::now();

        std::vector<Update> updates_in_chunk; // TODO: Make static array and do indexing instead of push_back

        // Comment out from here...
        for (graph_size_t j = start_line - 1; j < end_line - 1; j++) {
            // std::cout << "READING EDGE " << updates[j][0] << " " << updates[j][1] << std::endl;

            vertex_id_t src = updates[j][0];
            vertex_id_t dst = updates[j][1];

            //std::cout << src << " " << dst << std::endl;

            g->add_edge(src, dst);

            Update u(ADD, src, dst);
            updates_in_chunk.push_back(u);
        }
        // ... to here for switching between task-based and no-task updates


        // From here...
        /*
        for (graph_size_t j = start_line - 1; j < end_line - 1; j++) {
            // std::cout << "READING EDGE " << updates[j][0] << " " << updates[j][1] << std::endl;

            //std::cout << "BREAKPOINT 1" << std::endl;
            vertex_id_t src = updates[j][0];
            vertex_id_t dst = updates[j][1];

            //std::cout << "BREAKPOINT 2" << std::endl;
            // TODO: For now we only have edge additions, so hardcoding ADD type. Introduce edge removals.
            Update u(ADD, src, dst);
            updates_in_chunk.push_back(u);

            //std::cout << "BREAKPOINT 3" << std::endl;
            Task *task = static_cast<Task*>(task_pool::malloc());
            //Task *task = (Task*) malloc(sizeof(Task));

            task->task_type = UPDATE;
            task->timestamp_logical = g->get_incremented_global_logical_ts();
            //std::cout << task->timestamp_logical << std::endl;
            task->g = g;
            task->src = src;
            task->dst = dst;
            task->update_f = (u.type == ADD) ? updating.edge_insertion : updating.edge_deletion;

            //std::cout << "BREAKPOINT 4" << std::endl;
            GlobalScheduler::get_scheduler().submit(task);
        }


        GlobalScheduler::get_scheduler().barrier();

        GlobalScheduler::get_scheduler().task_counter = 0;
        */
        // ... to here

        // Write to file just to mark the iteration under which the barrier occurs
        //fs2 << std::chrono::duration<float>(std::chrono::system_clock::now() - GlobalScheduler::get_thread_pool().tp_start).count() << " " << GlobalScheduler::get_scheduler().iteration_counter << " " << g->get_order() << " " << g->get_size()<< " " << "-1" << " " << "-1" << std::endl;

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

        std::cout << "Computing CC..." << std::endl;
        std::chrono::steady_clock::time_point cpu_begin_cc = std::chrono::steady_clock::now();
        set_components_labels(g);
        std::chrono::steady_clock::time_point cpu_end_cc = std::chrono::steady_clock::now();
        std::cout << "Finished computing CC" << std::endl;
        float cpu_time_cc = std::chrono::duration<float>(cpu_end_cc - cpu_begin_cc).count();
        std::cout << "(CPU) CC TIME FOR chunk " << std::fixed << i+1 << ": " << cpu_time_cc << std::endl;

        std::cout << "Executing computations for " << updates_in_chunk.size() << " updates" << std::endl;

        // Execute user-defined compute functions (in form of ON_UPDATE and ON_ACTIVATE tasks)
        std::chrono::system_clock::time_point system_begin_compute = std::chrono::system_clock::now();
        std::chrono::steady_clock::time_point cpu_begin_compute = std::chrono::steady_clock::now();

        for (auto &u : updates_in_chunk) {
            Task *task = static_cast<Task*>(task_pool::malloc());
            //Task *task = (Task*) malloc(sizeof(Task));

            task->task_type = ON_UPDATE;
            task->timestamp_logical = g->get_incremented_global_logical_ts();
            task->g = g;
            task->src = u.src;
            task->dst = u.dst;
            task->edge_f = (u.type == ADD) ? computation.on_add_edge : computation.on_remove_edge;

            GlobalScheduler::get_scheduler().submit(task);

            // CC-based scheduling here
            components_number_t src_component = task->g->get_component_label(u.src);
            components_number_t dst_component = task->g->get_component_label(u.dst);

            if (u.type == REMOVE && src_component != dst_component) {

                std::cout << "YESS" << std::endl;

                //for every vertex in the component of src_component and dst.component schedule computation
                for (vertex_id_t vertex : cc_map[src_component]) {
                    Task *task = static_cast<Task*>(task_pool::malloc());
                    //Task *task = (Task*) malloc(sizeof(Task));

                    task->task_type = ON_ACTIVATE;
                    task->timestamp_logical = g->get_incremented_global_logical_ts();
                    task->g = g;
                    task->v = vertex;
                    task->vertex_f = computation.on_activate;

                    GlobalScheduler::get_scheduler().submit(task);
                }

                for (vertex_id_t vertex : cc_map[dst_component]) {
                    Task *task = static_cast<Task*>(task_pool::malloc());
                    //Task *task = (Task*) malloc(sizeof(Task));

                    task->task_type = ON_ACTIVATE;
                    task->timestamp_logical = g->get_incremented_global_logical_ts();
                    task->g = g;
                    task->v = vertex;
                    task->vertex_f = computation.on_activate;

                    GlobalScheduler::get_scheduler().submit(task);
                }
            }
        }

        GlobalScheduler::get_scheduler().barrier();

        GlobalScheduler::get_scheduler().task_counter = 0;

        // Write to file just to mark the iteration under which the barrier occurs
        //fs2 << std::chrono::duration<float>(std::chrono::system_clock::now() - GlobalScheduler::get_thread_pool().tp_start).count() << " " << GlobalScheduler::get_scheduler().iteration_counter << " " << g->get_order() << " " << g->get_size()<< " " << "-1" << " " << "-1" << std::endl;

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

    std::chrono::system_clock::time_point system_end_total = std::chrono::system_clock::now();
    std::chrono::steady_clock::time_point cpu_end_total = std::chrono::steady_clock::now();

    float system_time_total = std::chrono::duration<float>(system_end_total - system_begin_total).count();
    float cpu_time_total = std::chrono::duration<float>(cpu_end_total - cpu_begin_total).count();

    std::cout << "Finished all computations." << std::endl;
    std::cout << "(SYS) COMPUTE END-TO-END TIME: " << std::fixed << system_time_total << std::endl;
    std::cout << "(CPU) COMPUTE END-TO-END TIME: " << std::fixed << cpu_time_total << std::endl;
    std::cout << "--------------------------------------------------------------------------------" << std::endl;

    fs.close();

    fs.open("end-to-end-time.txt");
    fs << std::fixed << cpu_time_total << std::endl;
    fs.close();
}
