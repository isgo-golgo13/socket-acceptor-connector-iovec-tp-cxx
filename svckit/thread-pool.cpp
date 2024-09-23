#include "thread-pool.hpp"
#include <iostream>

ThreadPool::ThreadPool(size_t poolSize) : stop_(false) {
    // Initialize the pool of threads
    for (size_t i = 0; i < poolSize; ++i) {
        threads_.emplace_back(std::make_unique<Thread>([this]() { worker(); }));
        threads_.back()->start();
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        stop_ = true;
    }
    condition_.notify_all();  // Notify all threads to stop

    for (auto& thread : threads_) {
        if (thread->joinable()) {
            thread->join();  // Join all threads
        }
    }
}

void ThreadPool::enqueueTask(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex_);
        taskQueue_.emplace_back(std::move(task));
    }
    condition_.notify_one();  // Notify one worker thread that a task is available
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex_);

            // Wait for a task or to stop the pool
            condition_.wait(lock, [this]() { return !taskQueue_.empty() || stop_; });

            if (stop_ && taskQueue_.empty()) {
                return;  // Stop the thread if no tasks are left and stop flag is set
            }

            task = std::move(taskQueue_.front());
            taskQueue_.pop_front();  // Fetch the task
        }

        task();  // Execute the task
    }
}