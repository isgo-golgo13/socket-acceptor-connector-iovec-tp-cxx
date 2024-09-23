#pragma once

#include <sys/uio.h>
#include <sys/socket.h>
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
    ssize_t send(int fd, const std::vector<char>& buffer);
    ssize_t send(int fd, const std::vector<char>& buffer, std::chrono::milliseconds timeout);
    ssize_t sendn(int fd, const std::vector<char>& buffer, size_t n);
    ssize_t sendn(int fd, const std::vector<char>& buffer, size_t n, std::chrono::milliseconds timeout);

    ssize_t sendv(int fd, const struct iovec* vector_buf, int n);
    ssize_t sendv(int fd, const struct iovec* vector_buf, int n, std::chrono::milliseconds timeout);

    ssize_t recv(int fd, std::vector<char>& buffer);
    ssize_t recv(int fd, std::vector<char>& buffer, std::chrono::milliseconds timeout);
    ssize_t recvn(int fd, std::vector<char>& buffer, size_t n);
    ssize_t recvn(int fd, std::vector<char>& buffer, size_t n, std::chrono::milliseconds timeout);

    ssize_t recv_vec(int fd, struct iovec* vector_buf, int n);
    ssize_t recv_vec(int fd, struct iovec* vector_buf, int n, std::chrono::milliseconds timeout);

    // Public accessor for iovec vector
    std::vector<std::shared_ptr<struct iovec>>& get_iovec_vector();
    const std::vector<std::shared_ptr<struct iovec>>& get_iovec_vector() const;

private:
    int socket_fd_;
    std::vector<std::shared_ptr<struct iovec>> iovec_vector_;
};
