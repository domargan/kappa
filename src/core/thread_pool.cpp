#include <iostream>

#include "thread_pool.h"
#include "utils/threading.h"

ThreadPool::ThreadPool()
    : threads{},
      done{false},
      task_queue{},
      active{0} {}

ThreadPool::~ThreadPool() {
    destroy();
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
        task_queue.wait_pop(task, active);

        task->execute();
        task->release();

        --active;
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
