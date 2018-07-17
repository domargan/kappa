#ifndef KAPPA_TASK
#define KAPPA_TASK

#include <future>

class IThreadTask
{
public:
    IThreadTask(void) = default;
    virtual ~IThreadTask(void) = default;
    IThreadTask(const IThreadTask& rhs) = delete;
    IThreadTask& operator=(const IThreadTask& rhs) = delete;
    IThreadTask(IThreadTask&& other) = default;
    IThreadTask& operator=(IThreadTask&& other) = default;

    virtual void execute() = 0;
};

template <typename Func>
class ThreadTask: public IThreadTask
{
public:
    ThreadTask(Func&& func)
            :m_func{std::move(func)}
    {
    }

    ~ThreadTask(void) override = default;
    ThreadTask(const ThreadTask& rhs) = delete;
    ThreadTask& operator=(const ThreadTask& rhs) = delete;
    ThreadTask(ThreadTask&& other) = default;
    ThreadTask& operator=(ThreadTask&& other) = default;

    void execute() override
    {
        m_func();
    }

private:
    Func m_func;
};

template <typename T>
class TaskFuture
{
public:
    TaskFuture(std::future<T>&& future)
            :m_future{std::move(future)}
    {
    }

    TaskFuture(const TaskFuture& rhs) = delete;
    TaskFuture& operator=(const TaskFuture& rhs) = delete;
    TaskFuture(TaskFuture&& other) = default;
    TaskFuture& operator=(TaskFuture&& other) = default;
    ~TaskFuture(void)
    {
        if(m_future.valid())
        {
            m_future.get();
        }
    }

    auto get(void)
    {
        return m_future.get();
    }


private:
    std::future<T> m_future;
};

#endif //KAPPA_THREAD_POOL
