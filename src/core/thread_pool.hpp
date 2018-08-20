#ifndef KAPPA_THREAD_POOL
#define KAPPA_THREAD_POOL

#include <algorithm>
#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "task.h"
#include "task_queue.hpp"
#include "utils/threading.h"

class ThreadPool {
public:
    ThreadPool()
        : done{false},
          workQueue{},
          threads{} {}

    void init_threads(const std::uint32_t num_threads, const unsigned int offset = 0) {
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

    void init_numa_node(const uint node_no, const uint no_of_threads = 16) {
        try {
            for (uint i = 0; i < no_of_threads; ++i) {
                // Create thread
                threads.emplace_back(&ThreadPool::worker, this);

                // Pin thread
                pin_thread(node_no + (i * 4), threads[i]);

                std::cout << "Thread pinned to CPU " << node_no + (i * 4) << std::endl;
            }
        }
        catch (...) {
            destroy();

            throw;
        }
    }

    void init_numa_nodes(const std::vector<uint> nodes) {
        for (auto node : nodes) {
            init_numa_node(node);
        }
    }

    ThreadPool(const ThreadPool &rhs) = delete;

    ThreadPool &operator=(const ThreadPool &rhs) = delete;

    ~ThreadPool(void) {
        destroy();
    }

    template<typename F, typename ...Args>
    void submit(task_type_t task_type, F &&f, Args &&...args) {
        auto boundTask = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        using ResultType = std::result_of_t<decltype(boundTask)()>;
        using PackagedTask = std::packaged_task<ResultType()>;
        using TaskType = Task<PackagedTask>;

        PackagedTask task{std::move(boundTask)};

        futureQueue.push(std::future<void>{task.get_future()});
        workQueue.push(std::make_unique<TaskType>(task_type, std::move(task)));
    }

    void barrier() {
        while (!futureQueue.empty()) {
            std::future<void> future;

            if (futureQueue.waitPop(future)) {
                future.wait();
            }
        }
    }

private:
    // Methods
    void worker(void) {
        while (!done) {
            std::unique_ptr<TaskInterface> task{nullptr};

            if (workQueue.waitPop(task)) {
                task->execute();
            }
        }
    }

    void destroy(void) {
        done = true;
        workQueue.invalidate();

        for (auto &thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    // Members
    std::vector<std::thread> threads;
    std::atomic_bool done;
    ThreadSafeQueue<std::unique_ptr<TaskInterface>> workQueue;
    ThreadSafeQueue<std::future<void>> futureQueue;
};

#endif //KAPPA_THREAD_POOL
