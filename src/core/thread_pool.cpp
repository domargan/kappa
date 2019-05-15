#include <iostream>
#include <fstream>

#include "thread_pool.h"
#include "utils/threading.h"

ThreadPool::ThreadPool()
    : threads{},
      done{false},
      task_queue{1024*1024*128},
      //task_queue{1024*1024*512},
      active{0},
      task_counter{0},
      iteration_counter{0},
      tp_start(std::chrono::system_clock::now()) {
    fs.open("task_amount.txt");
}

ThreadPool::~ThreadPool() {
    //destroy();
}

void ThreadPool::init_threads(const std::uint32_t num_threads, const unsigned int offset) {
    try {
        for (std::uint32_t i = 0u; i < num_threads; ++i) {
            // Create thread
            threads.emplace_back(&ThreadPool::worker, this);

            // Pin thread
            pin_thread(i + offset, threads[i]);

            std::cout << "Thread pinned to CPU " << i + offset << std::endl;
        }
    }
    catch (...) {
        destroy();

        throw;
    }
}

void ThreadPool::init_numa_node(const uint node_no, const uint no_of_threads) {
    try {
        for (uint i = 0; i < no_of_threads; ++i) {
            // Create thread
            threads.emplace_back(&ThreadPool::worker, this);

            // Pin thread
            pin_thread(node_no + (i * 4), threads.back());

            std::cout << "Thread pinned to CPU " << node_no + (i * 4) << std::endl;
        }
    }
    catch (...) {
        destroy();

        throw;
    }
}

void ThreadPool::init_numa_nodes(const std::vector<uint> nodes) {
    for (auto node : nodes) {
        init_numa_node(node);
    }
}

void ThreadPool::submit(Task *task) {
    task_queue.push(task);

    ++task_counter;
    ++iteration_counter;

    {
        std::lock_guard<std::mutex> lock{mtx};

        int task_type;

        if (task->task_type == VERTEX) { task_type = 0; } // On activate
        else if (task->task_type == EDGE) { task_type = 1; } // On add edge
        else if (task->task_type == UPDATE) { task_type = 2; } // Insert/delete edge

        if (task_counter % 100 == 0) { // or 1000 when having more inputs
            fs << std::chrono::duration<float>(std::chrono::system_clock::now() - tp_start).count() << " " // Real time
                << iteration_counter << " " // Logical time (iterations)
                << task->g->get_order() << " "
                << task->g->get_size() << " "
                << task_counter << " "
                << task_type << " "
                << std::endl;
        }
    }
}

void ThreadPool::barrier() {
    while (true) {
        task_queue.wait_empty();

        if (active == 0 && task_queue.empty()) {
            return;
        }
    }
}

void ThreadPool::worker(void) {
    while (!done) {
        Task *task;
        task_queue.pop(task, active);

        task->execute();
        task->release();

        --active;
        --task_counter;
    }
}

void ThreadPool::destroy(void) {
    done = true;

    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}
