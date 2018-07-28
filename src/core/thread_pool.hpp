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
    ThreadPool(void)
            : ThreadPool{std::max(std::thread::hardware_concurrency(), 2u) - 1u} {}

    explicit ThreadPool(const std::uint32_t numThreads, const unsigned int offset = 0)
            : done{false},
              workQueue{},
              threads{} {
        try {
            for (std::uint32_t i = 0u; i < numThreads; ++i) {
                // Create thread
                threads.emplace_back(&ThreadPool::worker, this);

                // Pin thread to CPU i + offset
                pin_thread(i + offset, threads[i]);
            }
        }
        catch (...) {
            destroy();

            throw;
        }
    }

    ThreadPool(const ThreadPool &rhs) = delete;

    ThreadPool &operator=(const ThreadPool &rhs) = delete;

    ~ThreadPool(void) {
        destroy();
    }

    template<typename F, typename... Args>
    auto submit(F &&f, Args &&... args) {
        auto boundTask = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        using ResultType = std::result_of_t<decltype(boundTask)()>;
        using PackagedTask = std::packaged_task<ResultType()>;
        using TaskType = Task<PackagedTask>;

        PackagedTask task{std::move(boundTask)};
        TaskFuture<ResultType> result{task.get_future()};
        workQueue.push(std::make_unique<TaskType>(std::move(task)));

        return result;
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
};

#endif //KAPPA_THREAD_POOL
