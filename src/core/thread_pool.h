#ifndef KAPPA_THREAD_POOL
#define KAPPA_THREAD_POOL

#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>

#include "task.h"
#include "thread_safe_queue.hpp"
#include "mpmc_queue.h"

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool &operator=(const ThreadPool&) = delete;

    void init_threads(const std::uint32_t, const unsigned int offset = 0);
    void init_numa_node(const uint, const uint no_of_threads = 16);
    void init_numa_nodes(const std::vector<uint>);

    void submit(Task*);
    void barrier();

    std::atomic_uint task_counter;

private:
    void worker(void);
    void destroy(void);

    std::vector<std::thread> threads;
    std::atomic_bool done;
    //ThreadSafeQueue<Task*> task_queue;
    MPMCQueue<Task*> task_queue;
    std::atomic_ushort active;
};

#endif //KAPPA_THREAD_POOL
