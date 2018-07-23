#ifndef KAPPA_TASK_QUEUE_H
#define KAPPA_TASK_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template<typename T>
class ThreadSafeQueue {
public:
    ~ThreadSafeQueue(void) {
        invalidate();
    }

    bool tryPop(T &out) {
        std::lock_guard<std::mutex> lock{mutex};

        if (queue.empty() || !valid) {
            return false;
        }

        out = std::move(queue.front());
        queue.pop();

        return true;
    }

    bool waitPop(T &out) {
        std::unique_lock<std::mutex> lock{mutex};

        condition.wait(lock, [this]() {
            return !queue.empty() || !valid;
        });

        if (!valid) {
            return false;
        }

        out = std::move(queue.front());
        queue.pop();

        return true;
    }

    void push(T value) {
        std::lock_guard<std::mutex> lock{mutex};

        queue.push(std::move(value));
        condition.notify_one();
    }

    bool empty(void) const {
        std::lock_guard<std::mutex> lock{mutex};

        return queue.empty();
    }

    void clear(void) {
        std::lock_guard<std::mutex> lock{mutex};

        while (!queue.empty()) {
            queue.pop();
        }

        condition.notify_all();
    }

    void invalidate(void) {
        std::lock_guard<std::mutex> lock{mutex};

        valid = false;
        condition.notify_all();
    }

    bool isValid(void) const {
        std::lock_guard<std::mutex> lock{mutex};

        return valid;
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::atomic_bool valid{true};
    std::condition_variable condition;
};

#endif //KAPPA_TASK_QUEUE_H
