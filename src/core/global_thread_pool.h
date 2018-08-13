#ifndef KAPPA_GLOBAL_THREAD_POOL_H
#define KAPPA_GLOBAL_THREAD_POOL_H

#include "thread_pool.hpp"

namespace GlobalThreadPool {
    inline ThreadPool &get_thread_pool() {
        static ThreadPool threadPool(get_no_of_cpus() - NON_WORKER_THREADS, NON_WORKER_THREADS);

        return threadPool;
    }
}

#endif //KAPPA_GLOBAL_THREAD_POOL_H
