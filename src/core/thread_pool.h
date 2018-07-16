#ifndef KAPPA_THREAD_POOL_H
#define KAPPA_THREAD_POOL_H

#include "thread_safe_queue.h"

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

class ThreadPool {
private:
    class IThreadTask {
    public:
        IThreadTask();
        virtual ~IThreadTask();
        IThreadTask(const IThreadTask&);
        IThreadTask& operator=(const IThreadTask&);
        IThreadTask(IThreadTask&&);
        IThreadTask& operator=(IThreadTask&&);
        virtual void execute();
    };

    template <typename Func>
    class ThreadTask: public IThreadTask {
    private:
        Func m_func;
    public:
        ThreadTask(Func&&);
        ~ThreadTask() override;
        ThreadTask(const ThreadTask&);
        ThreadTask& operator=(const ThreadTask&);
        ThreadTask(ThreadTask&&);
        ThreadTask& operator=(ThreadTask&&);
        void execute() override;
    };

public:
    template <typename T>
    class TaskFuture {
    private:
        std::future<T> m_future;
    public:
        TaskFuture(std::future<T>&&);
        TaskFuture(const TaskFuture&);
        TaskFuture& operator=(const TaskFuture&);
        TaskFuture(TaskFuture&&);
        TaskFuture& operator=(TaskFuture&&);
        ~TaskFuture();
        auto get();
    };

public:
    ThreadPool();
    explicit ThreadPool(const std::uint32_t);
    ThreadPool(const ThreadPool&);
    ThreadPool& operator=(const ThreadPool&);
    ~ThreadPool();
    template <typename Func, typename... Args>
    auto submit(Func&&, Args&&...);

private:
    void worker();
    void destroy();

private:
    std::atomic_bool m_done;
    ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
    std::vector<std::thread> m_threads;
};


inline ThreadPool& getThreadPool();
template <typename Func, typename... Args>
inline auto submitJob(Func&&, Args&&...);

#endif //KAPPA_THREAD_POOL_H
