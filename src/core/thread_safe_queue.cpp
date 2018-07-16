#include "thread_safe_queue.h"

template<typename T>
ThreadSafeQueue<T>::ThreadSafeQueue() {
    std::atomic_bool m_valid{true};
    mutable std::mutex m_mutex;

    template <typename T>
    std::queue<T> m_queue;

    std::condition_variable m_condition;
}

template<typename T>
ThreadSafeQueue<T>::~ThreadSafeQueue() {
    invalidate();
}

template<typename T>
bool ThreadSafeQueue<T>::tryPop(T & out) {
    std::lock_guard<std::mutex> lock{m_mutex};
    if(m_queue.empty() || !m_valid) {
        return false;
    }
    out = std::move(m_queue.front());
    m_queue.pop();
    return true;}

template<typename T>
bool ThreadSafeQueue<T>::waitPop(T& out) {
    std::unique_lock<std::mutex> lock{m_mutex};
    m_condition.wait(lock, [this]() {
        return !m_queue.empty() || !m_valid;
    });

    if(!m_valid) {
        return false;
    }
    out = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::push(T value) {
    std::lock_guard<std::mutex> lock{m_mutex};
    m_queue.push(std::move(value));
    m_condition.notify_one();
}

bool ThreadSafeQueue::empty() const {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_queue.empty();
}

void ThreadSafeQueue::clear() {
    std::lock_guard<std::mutex> lock{m_mutex};
    while(!m_queue.empty()) {
        m_queue.pop();
    }
    m_condition.notify_all();
}

void ThreadSafeQueue::invalidate() {
    std::lock_guard<std::mutex> lock{m_mutex};
    m_valid = false;
    m_condition.notify_all();
}

bool ThreadSafeQueue::isValid() const {
    std::lock_guard<std::mutex> lock{m_mutex};
    return m_valid;
}

