#ifndef KAPPA_TASK_H
#define KAPPA_TASK_H

#include <atomic>
#include <chrono>
#include <future>
#include <iostream>
#include <string>

#include "datatypes.h"
#include "digraph.h"

class TaskInterface {
public:
    virtual void execute() = 0;
};

template<typename F>
class Task : public TaskInterface {
public:
    Task(task_type_t task_type, F &&f)
        : task_type{task_type}
        , f{std::move(f)} {
        this->timestamp_real = std::chrono::duration_cast<timestamp_real_t>(
            std::chrono::system_clock::now().time_since_epoch()
        );
        this->timestamp_logical = get_counter()++;
    }

    ~Task(void) = default;

    Task(const Task &rhs) = delete;

    Task &operator=(const Task &rhs) = delete;

    Task(Task &&other) = default;

    Task &operator=(Task &&other) = default;

    void execute() {
        f();
    }

    task_type_t get_task_type() {
        return task_type;
    }

    timestamp_real_t get_timestamp_real() {
        return timestamp_real;
    }

    timestamp_logical_t get_timestamp_logical() {
        return timestamp_logical;
    }

    void set_task_type(task_type_t task_type) {
        this->task_type = task_type;
    }

    void set_timestamp_real(timestamp_real_t timestamp_real) {
        this->timestamp_real = timestamp_real;
    }

    void set_timestamp_logical(timestamp_logical_t timestamp_logical) {
        this->timestamp_logical = timestamp_logical;
    }

    friend std::ostream &operator<<(std::ostream &out, const Task &task) {
        std::string task_enum;

        switch (task.task_type) {
            case COMPUTE:
                task_enum = "COMPUTE";
                break;
            case UPDATE:
                task_enum = "UPDATE";
        }

        return out << task_enum << " task @" <<
            " logical=" << task.timestamp_logical <<
            " real=" << task.timestamp_real.count();
    }

private:
    // TODO: Turn me into static class variable
    std::atomic<timestamp_logical_t> &get_counter() {
        static std::atomic<timestamp_logical_t> counter{0};

        return counter;
    }

    F f;

    task_type_t task_type;

    timestamp_real_t timestamp_real;
    timestamp_logical_t timestamp_logical;
};

#endif //KAPPA_TASK_H
