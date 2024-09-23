#include "thread.hpp"

Thread::Thread(std::function<void()> func) : func_(std::move(func)) {}

void Thread::start() {
    thread_ = std::thread(func_);
}

void Thread::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool Thread::joinable() const {
    return thread_.joinable();
}