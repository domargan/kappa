#include "runtime_hard_barrier.h"

#include <ctime>
#include <fstream>
#include <iomanip>

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
    std::cout << "\n-----------------------------------------------------------------------------------------"
                 "\n[START]\t\tSTARTING RUNTIME..." << std::endl;

    std::ofstream fs;
    fs.open("measurements.csv");
    fs << "Order" << " , " << "Size" << " , " << "Ingestion Rate" << " , " << "Ingestion CPU Time" << " , " << "Computation CPU time" << std::endl;

    //std::ofstream fs2;
    //fs2.open("task_amount.txt");

    graph_size_t num_of_chunks = chunks_start_lines.size() - 1; //-1 because the last element is just there to mark the end point

    graph_size_t start_line = 0;
    graph_size_t end_line = 0;

    // Reset start time for queue evaluation
    //GlobalScheduler::get_scheduler().tp_start = std::chrono::steady_clock::now();

    // Compute CC for core graph
    set_components_labels(g);

    // Measure end-to-end time
    std::chrono::steady_clock::time_point timer_start_total = std::chrono::steady_clock::now();

    for (graph_size_t i = 0; i < num_of_chunks; ++i) {
        start_line = chunks_start_lines[i];
        end_line = chunks_start_lines[i+1];

        std::cout << "\n[START]\t\tReading updates from chunk " << i+1 << std::endl;

        // Update the graph
        std::chrono::steady_clock::time_point timer_start_update = std::chrono::steady_clock::now();

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
        //fs2 << std::chrono::duration<float>(std::chrono::steady_clock::now() - GlobalScheduler::get_thread_pool().tp_start).count() << " " << GlobalScheduler::get_scheduler().iteration_counter << " " << g->get_order() << " " << g->get_size()<< " " << "-1" << " " << "-1" << std::endl;

        std::chrono::steady_clock::time_point timer_end_update = std::chrono::steady_clock::now();

        float timer_update = std::chrono::duration<float>(timer_end_update - timer_start_update).count();

        float ingestion_rate = (end_line-start_line) / timer_update;

        std::cout << "[END]\t\tFinished updating from chunk\t\t\t\t\t" << i+1 << std::endl;
        std::cout << "[TIME]\t\tIngesting updates from chunk " << std::fixed << i+1 << ":\t\t\t\t\t" << timer_update << std::endl;

        // g->count_order();
        graph_size_t order = g->get_order();
        graph_size_t size = g->get_size();
        std::cout << "[INFO]\t\tOrder:\t\t\t\t\t\t\t\t" <<  order << std::endl;
        std::cout << "[INFO]\t\tSize:\t\t\t\t\t\t\t\t" << size << std::endl;

        set_components_labels(g);

        std::cout << "\n[START]\t\tExecuting computations for " << updates_in_chunk.size() << " updates" << std::endl;

        // Execute user-defined compute functions (in form of ON_UPDATE and ON_ACTIVATE tasks)
        std::chrono::steady_clock::time_point timer_start_compute = std::chrono::steady_clock::now();

        GlobalScheduler::get_scheduler().start_workers();

        for (auto &u : updates_in_chunk) {
            Task *task = static_cast<Task*>(task_pool::malloc());
            //Task *task = (Task*) malloc(sizeof(Task));

            task->task_type = ON_UPDATE;
            //task->timestamp_logical = g->get_incremented_global_logical_ts();
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
                    //task->timestamp_logical = g->get_incremented_global_logical_ts();
                    task->g = g;
                    task->v = vertex;
                    task->vertex_f = computation.on_activate;

                    GlobalScheduler::get_scheduler().submit(task);
                }

                for (vertex_id_t vertex : cc_map[dst_component]) {
                    Task *task = static_cast<Task*>(task_pool::malloc());
                    //Task *task = (Task*) malloc(sizeof(Task));

                    task->task_type = ON_ACTIVATE;
                    //task->timestamp_logical = g->get_incremented_global_logical_ts();
                    task->g = g;
                    task->v = vertex;
                    task->vertex_f = computation.on_activate;

                    GlobalScheduler::get_scheduler().submit(task);
                }
            }
        }

        GlobalScheduler::get_scheduler().barrier();
        GlobalScheduler::get_scheduler().halt_workers();

        //GlobalScheduler::get_scheduler().task_counter = 0;

        // Write to file just to mark the iteration under which the barrier occurs
        //fs2 << std::chrono::duration<float>(std::chrono::steady_clock::now() - GlobalScheduler::get_thread_pool().tp_start).count() << " " << GlobalScheduler::get_scheduler().iteration_counter << " " << g->get_order() << " " << g->get_size()<< " " << "-1" << " " << "-1" << std::endl;

        std::chrono::steady_clock::time_point timer_end_compute = std::chrono::steady_clock::now();

        float timer_compute = std::chrono::duration<float>(timer_end_compute - timer_start_compute).count();

        std::cout << "[END]\t\tFinished computations." << std::endl;
        std::cout << "[TIME]\t\tComputing for chunk " << std::fixed << i+1 << ":\t\t\t\t\t\t" << timer_compute << std::endl;

        fs << order << " , " << size << " , " << std::fixed << ingestion_rate << " , " << std::fixed << timer_update << " , " << std::fixed << timer_compute << std::endl;
    }

    std::chrono::steady_clock::time_point timer_end_total = std::chrono::steady_clock::now();

    float timer_total = std::chrono::duration<float>(timer_end_total - timer_start_total).count();

    std::cout << "\n[END]\t\tFinished all computations." << std::endl;
    std::cout << "[TIME]\t\tEnd-to-end computations:\t\t\t\t\t" << std::fixed << timer_total << std::endl;

    fs.close();
    //fs2.close();

    fs.open("end-to-end-time.txt");
    fs << std::fixed << timer_total << std::endl;
    fs.close();

    std::cout << "\n[END]\t\tEnding runtime..." << std::endl;
}
