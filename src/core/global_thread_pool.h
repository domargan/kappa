#ifndef KAPPA_GLOBAL_THREAD_POOL_H
#define KAPPA_GLOBAL_THREAD_POOL_H

#include "scheduler.h"

namespace GlobalThreadPool {
    inline Scheduler &get_thread_pool() {
        static Scheduler threadPool;

        return threadPool;
    }
}

#endif //KAPPA_GLOBAL_THREAD_POOL_H
