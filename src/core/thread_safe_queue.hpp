#ifndef KAPPA_THREAD_SAFE_QUEUE_H
#define KAPPA_THREAD_SAFE_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template<typename T>
class ThreadSafeQueue {
public:
    void wait_pop(T &out, std::atomic_ushort &active) {
        std::unique_lock<std::mutex> lock{mutex};

        push_condition.wait(lock, [this]() {
            return !queue.empty();
        });

        out = std::move(queue.front());
        queue.pop();

        ++active;

        if (queue.empty()) {
            empty_condition.notify_all();
        }
    }

    void push(T value) {
        std::lock_guard<std::mutex> lock{mutex};

        queue.push(std::move(value));
        push_condition.notify_one();
    }

    bool empty(void) const {
        std::lock_guard<std::mutex> lock{mutex};

        return queue.empty();
    }

    void wait_empty(void) {
        std::unique_lock<std::mutex> lock{mutex};

        empty_condition.wait(lock, [this]() {
            return queue.empty();
        });
    }

    void clear(void) {
        std::lock_guard<std::mutex> lock{mutex};

        while (!queue.empty()) {
            queue.pop();
        }

        push_condition.notify_all();
    }

private:
    std::queue<T> queue;
    mutable std::mutex mutex;
    std::condition_variable push_condition;
    std::condition_variable empty_condition;
};

#endif //KAPPA_THREAD_SAFE_QUEUE_H
