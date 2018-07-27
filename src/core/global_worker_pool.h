#ifndef KAPPA_GLOBAL_WORKER_POOL_H
#define KAPPA_GLOBAL_WORKER_POOL_H

#include "thread_pool.hpp"

namespace GlobalWorkerPool {
    inline ThreadPool &getThreadPool(void) {
        static ThreadPool globalWorkerPool;

        return globalWorkerPool;
    }
}

#endif //KAPPA_GLOBAL_WORKER_POOL_H
