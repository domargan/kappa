#ifndef KAPPA_TASK_H
#define KAPPA_TASK_H

#include <future>
#include "datatypes.h"
#include "digraph.h"

class TaskInterface {
public:
    virtual void execute() = 0;
};

template<typename F>
class Task : public TaskInterface {
    /*
    timestamp_ms_t timestamp_ms;
    timestap_logical_t timestap_logical;

    Task(void(*)(vertex_id_t, Digraph*)) { }

    void set_task_type();

    void set_timestamp_ms();
    void set_timestamp_logical();

    void set_task_function();
    void set_task_data();
    */

public:
    Task(F &&f)
            : f{std::move(f)} {}

    ~Task(void) = default;

    Task(const Task &rhs) = delete;

    Task &operator=(const Task &rhs) = delete;

    Task(Task &&other) = default;

    Task &operator=(Task &&other) = default;

    void execute() {
        f();
    }

private:
    F f;
};

template<typename T>
class TaskFuture {
public:
    TaskFuture(std::future<T> &&future)
            : future{std::move(future)} {}

    TaskFuture(const TaskFuture &rhs) = delete;

    TaskFuture &operator=(const TaskFuture &rhs) = delete;

    TaskFuture(TaskFuture &&other) = default;

    TaskFuture &operator=(TaskFuture &&other) = default;

    ~TaskFuture(void) {
        if (future.valid()) {
            future.get();
        }
    }

    auto get(void) {
        return future.get();
    }

private:
    std::future<T> future;
};

#endif //KAPPA_TASK_H
