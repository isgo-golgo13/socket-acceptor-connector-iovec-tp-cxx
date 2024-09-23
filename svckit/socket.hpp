#pragma once

#include <sys/uio.h>    // For iovec
#include <sys/socket.h> // For socket functions
#include <chrono>
#include <vector>
#include <memory>
#include <cstring>
#include <iostream>

class Socket {
public:
    Socket() = default;
    explicit Socket(int fd);
    Socket(const Socket& other) = delete;
    Socket(Socket&& other) noexcept;
    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other) noexcept;
    ~Socket();

    // Send and receive methods
    ssize_t send(int fd, const void* buffer, size_t length);
    ssize_t send(int fd, const void* buffer, size_t length, std::chrono::milliseconds timeout);
    ssize_t sendn(int fd, const void* buffer, size_t n);
    ssize_t sendn(int fd, const void* buffer, size_t n, std::chrono::milliseconds timeout);

    ssize_t sendv(int fd, const struct iovec* vector_buf, int n);
    ssize_t sendv(int fd, const struct iovec* vector_buf, int n, std::chrono::milliseconds timeout);

    ssize_t recv(int fd, void* buffer, size_t length);
    ssize_t recv(int fd, void* buffer, size_t length, std::chrono::milliseconds timeout);
    ssize_t recvn(int fd, void* buffer, size_t n);
    ssize_t recvn(int fd, void* buffer, size_t n, std::chrono::milliseconds timeout);

    ssize_t recv_vec(int fd, struct iovec* vector_buf, int n);
    ssize_t recv_vec(int fd, struct iovec* vector_buf, int n, std::chrono::milliseconds timeout);

    // A vector of shared pointers to iovec
    std::vector<std::shared_ptr<struct iovec>> iovec_vector_;

private:
    int socket_fd_;
};
