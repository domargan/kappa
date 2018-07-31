#ifndef KAPPA_SCHEDULER_H
#define KAPPA_SCHEDULER_H

#include "task.h"
#include "task_queue.hpp"
#include "thread_pool.hpp"

typedef ThreadSafeQueue<std::unique_ptr<TaskInterface>> task_queue;
typedef int task_id;

class Scheduler {
private:
    task_queue tasks_waiting;

    void submit_task_to_waiting(TaskInterface*);
    void submit_task_to_runnable(TaskInterface*);
    void move_task_to_runnable(TaskInterface*);

    void update_dependencies_global();
    void update_dependencies(task_id);
    void check_dependency(task_id);
    void notify_dependants(task_id);
public:
    explicit Scheduler(ThreadPool*);
};

#endif //KAPPA_SCHEDULER_H
