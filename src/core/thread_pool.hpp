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
#include "task_queue.hpp"
#include "task.h"

class ThreadPool {
public:
    ThreadPool(void)
            : ThreadPool{std::max(std::thread::hardware_concurrency(), 2u) - 1u} {}

    explicit ThreadPool(const std::uint32_t numThreads)
            : done{false},
              workQueue{},
              threads{} {
        try {
            for (std::uint32_t i = 0u; i < numThreads; ++i) {
                threads.emplace_back(&ThreadPool::worker, this);
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

namespace DefaultThreadPool {
    inline ThreadPool &getThreadPool(void) {
        static ThreadPool defaultPool;

        return defaultPool;
    }
}

#endif //KAPPA_THREAD_POOL
