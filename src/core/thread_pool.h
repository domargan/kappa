#ifndef KAPPA_THREAD_POOL
#define KAPPA_THREAD_POOL

#include "task_queue.h"
#include "task.h"

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


class ThreadPool
{
public:
    ThreadPool(void)
            :ThreadPool{std::max(std::thread::hardware_concurrency(), 2u) - 1u}
    {
    }

    explicit ThreadPool(const std::uint32_t numThreads)
            :m_done{false},
             m_workQueue{},
             m_threads{}
    {
        try
        {
            for(std::uint32_t i = 0u; i < numThreads; ++i)
            {
                m_threads.emplace_back(&ThreadPool::worker, this);
            }
        }
        catch(...)
        {
            destroy();
            throw;
        }
    }

    ThreadPool(const ThreadPool& rhs) = delete;

    ThreadPool& operator=(const ThreadPool& rhs) = delete;

    ~ThreadPool(void)
    {
        destroy();
    }

    template <typename Func, typename... Args>
    auto submit(Func&& func, Args&&... args)
    {
        auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        using ResultType = std::result_of_t<decltype(boundTask)()>;
        using PackagedTask = std::packaged_task<ResultType()>;
        using TaskType = ThreadTask<PackagedTask>;

        PackagedTask task{std::move(boundTask)};
        TaskFuture<ResultType> result{task.get_future()};
        m_workQueue.push(std::make_unique<TaskType>(std::move(task)));
        return result;
    }

private:
    void worker(void)
    {
        while(!m_done)
        {
            std::unique_ptr<IThreadTask> pTask{nullptr};
            if(m_workQueue.waitPop(pTask))
            {
                pTask->execute();
            }
        }
    }

    void destroy(void)
    {
        m_done = true;
        m_workQueue.invalidate();
        for(auto& thread : m_threads)
        {
            if(thread.joinable())
            {
                thread.join();
            }
        }
    }

private:
    std::atomic_bool m_done;
    ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
    std::vector<std::thread> m_threads;
};

namespace DefaultThreadPool
{
    inline ThreadPool& getThreadPool(void)
    {
        static ThreadPool defaultPool;
        return defaultPool;
    }

    template <typename Func, typename... Args>
    inline auto submitJob(Func&& func, Args&&... args)
    {
        return getThreadPool().submit(std::forward<Func>(func), std::forward<Args>(args)...);
    }
}


#endif //KAPPA_THREAD_POOL
