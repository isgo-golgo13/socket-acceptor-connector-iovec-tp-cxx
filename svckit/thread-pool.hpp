#pragma once

#include "thread.hpp"
#include <functional>
#include <vector>
#include <deque>
#include <memory>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    explicit ThreadPool(size_t poolSize);
    ~ThreadPool();

    // Add a new task to the queue
    void enqueueTask(std::function<void()> task);

private:
    // Worker function for each thread in the pool
    void worker();

    std::vector<std::unique_ptr<Thread>> threads_;        // Pool of threads
    std::deque<std::function<void()>> taskQueue_;         // Queue of tasks
    std::mutex queueMutex_;                               // Mutex to protect taskQueue_
    std::condition_variable condition_;                   // Condition variable to notify worker threads
    bool stop_;                                           // Flag to stop the pool
};