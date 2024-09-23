#pragma once

#include <thread>
#include <functional>

class Thread {
public:
    explicit Thread(std::function<void()> func);
    Thread() = delete;
    Thread(const Thread& other) = delete;
    Thread(Thread&& other) noexcept = default;
    Thread& operator=(const Thread& other) = delete;
    Thread& operator=(Thread&& other) noexcept = default;
    ~Thread() = default;

    void start();
    void join();
    bool joinable() const;

private:
    std::function<void()> func_;
    std::thread thread_;
};