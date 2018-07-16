#ifndef KAPPA_THREAD_SAFE_QUEUE_H
#define KAPPA_THREAD_SAFE_QUEUE_H

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

template <typename T>
class ThreadSafeQueue {
private:
    std::atomic_bool m_valid;
    mutable std::mutex m_mutex;
    std::queue<T> m_queue;
    std::condition_variable m_condition;

public:
    ThreadSafeQueue();
    ~ThreadSafeQueue();
    bool tryPop(T&);
    bool waitPop(T&);
    void push(T);
    bool empty() const;
    void clear();
    void invalidate();
    bool isValid() const;
};

#endif //KAPPA_THREAD_SAFE_QUEUE_H
