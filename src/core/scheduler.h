#ifndef KAPPA_SCHEDULER
#define KAPPA_SCHEDULER

#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>
#include <fstream>

#include "task.h"
#include "thread_safe_queue.hpp"
#include "mpmc_queue.h"

// Called ThreadPool before...
class Scheduler {
public:
    Scheduler();
    ~Scheduler();
    Scheduler(const Scheduler&) = delete;
    Scheduler &operator=(const Scheduler&) = delete;

    void init_threads(const std::uint32_t, const unsigned int offset = 0);
    void init_numa_node(const uint, const uint no_of_threads = 16);
    void init_numa_nodes(const std::vector<uint>);

    void submit(Task*);
    void barrier();

    std::atomic_uint iteration_counter;
    std::atomic_uint task_counter;

    std::chrono::system_clock::time_point tp_start;

private:
    void worker(void);
    void destroy(void);

    std::vector<std::thread> thread_pool;
    std::atomic_bool done;
    //ThreadSafeQueue<Task*> task_queue;
    MPMCQueue<Task*> task_queue;
    std::atomic_ushort active;

    std::mutex mtx;
    std::ofstream fs;

    bool check_task_dependency(Task*);

};

#endif //KAPPA_SCHEDULER
