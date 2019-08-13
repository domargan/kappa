#ifndef KAPPA_GLOBAL_SCHEDULER_H
#define KAPPA_GLOBAL_SCHEDULER_H

#include "scheduler.h"

namespace GlobalScheduler {
    inline Scheduler &get_scheduler() {
        static Scheduler scheduler;

        return scheduler;
    }
}

#endif //KAPPA_GLOBAL_SCHEDULER_H
