#ifndef KAPPA_GLOBAL_THREAD_POOL_H
#define KAPPA_GLOBAL_THREAD_POOL_H

#include "thread_pool.hpp"

namespace GlobalThreadPool {
    inline ThreadPool &get_thread_pool() {
        static ThreadPool threadPool;

        return threadPool;
    }
}

#endif //KAPPA_GLOBAL_THREAD_POOL_H
