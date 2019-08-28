#include <iostream>
#include <fstream>

#include "scheduler.h"
#include "utils/threading.h"

// Called ThreadPool before...
Scheduler::Scheduler()
    : thread_pool{},
      done{false},
      task_queue{1024*1024*128},
      //task_queue{1024*1024*512},
      active{0},
      task_counter{0},
      iteration_counter{0},
      tp_start(std::chrono::steady_clock::now()) {
    fs.open("task_amount.txt");
}

Scheduler::~Scheduler() {
    //destroy();
}

void Scheduler::init_threads(const std::uint32_t num_threads, const unsigned int offset) {
    try {
        for (std::uint32_t i = 0u; i < num_threads; ++i) {
            // Create thread
            thread_pool.emplace_back(&Scheduler::worker, this);

            // Pin thread
            pin_thread(i + offset, thread_pool[i]);

            std::cout << "[INFO]\t\tThread pinned to CPU " << i + offset << std::endl;
        }
    }
    catch (...) {
        destroy();

        throw;
    }
}

void Scheduler::init_numa_node(const uint node_no, const uint no_of_threads) {
    try {
        for (uint i = 0; i < no_of_threads; ++i) {
            // Create thread
            thread_pool.emplace_back(&Scheduler::worker, this);

            // Pin thread
            pin_thread(node_no + (i * 4), thread_pool.back());

            std::cout << "[INFO]\t\tThread pinned to CPU " << node_no + (i * 4) << std::endl;
        }
    }
    catch (...) {
        destroy();

        throw;
    }
}

void Scheduler::init_numa_nodes(const std::vector<uint> nodes) {
    for (auto node : nodes) {
        init_numa_node(node);
    }
}

bool Scheduler::check_task_dependency(Task *task) {
    vertex_id_t v = task->v;
    vertex_id_t src = task->src;
    vertex_id_t dst = task->dst;

    // TODO Finish this

    return true;
}

void Scheduler::submit(Task *task) {
    task_queue.push(task);

    ++task_counter;
    ++iteration_counter;

    /*
    // TODO: Remove this block for measurements
    {
        std::lock_guard<std::mutex> lock{mtx};

        int task_type;

        if (task->task_type == ON_ACTIVATE) { task_type = 0; } // On activate
        else if (task->task_type == ON_UPDATE) { task_type = 1; } // On add/remove edge
        else if (task->task_type == UPDATE) { task_type = 2; } // Insert/delete edge

        if (task_counter % 100 == 0) { // or 1000 when having more inputs
            fs << std::chrono::duration<float>(std::chrono::steady_clock::now() - tp_start).count() << " " // Real time
                << iteration_counter << " " // Logical time (iterations)
                << task->g->get_order() << " "
                << task->g->get_size() << " "
                << task_counter << " "
                << task_type << " "
                << std::endl;
        }
    }
     */
}

void Scheduler::barrier() {
    while (true) {
        task_queue.wait_empty();

        if (active == 0 && task_queue.empty()) {
            return;
        }
    }
}

void Scheduler::worker(void) {
    while (!done) {
        Task *task;
        task_queue.pop(task, active);

        //std::cout << task->task_type << " " << task->timestamp_logical <<  " " << task_counter << std::endl;

        task->execute();

        switch (task->task_type) {
            case ON_ACTIVATE:                    // On activate (basically "compute")
                task->g->set_vertex_on_activate_ts(task->v, task->timestamp_logical);

                break;
            case ON_UPDATE:                      // What to do after adding/removing and edge (e.g. run "activate")
                task->g->set_vertex_on_update_ts(task->src, task->timestamp_logical);
                task->g->set_vertex_on_update_ts(task->dst, task->timestamp_logical);
                // TODO: Introduce blocking logic for correctness

                break;
            case UPDATE:                    // Just removing and adding edges
                //std::cout << "UPDATE" << std::endl;
                //std::cout << task->timestamp_logical << std::endl;
                //std::cout << task->g->get_vertex_update_ts(task->v) << std::endl;
                task->g->set_vertex_update_ts(task->src, task->timestamp_logical);
                task->g->set_vertex_update_ts(task->dst, task->timestamp_logical);
                // TODO: Introduce blocking logic for correctness

                break;
        }

        task->release();

        --active;
        --task_counter;
    }
}

void Scheduler::destroy(void) {
    done = true;

    for (auto &thread : thread_pool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
