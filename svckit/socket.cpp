#include "socket.hpp"
#include <unistd.h>
#include <sys/select.h>
#include <iostream>

// Constructor
Socket::Socket(int fd) : socket_fd_(fd) {}

// Move constructor
Socket::Socket(Socket&& other) noexcept : socket_fd_(other.socket_fd_) {
    other.socket_fd_ = -1;
}

// Move assignment operator
Socket& Socket::operator=(Socket&& other) noexcept {
    if (this != &other) {
        socket_fd_ = other.socket_fd_;
        other.socket_fd_ = -1;
    }
    return *this;
}

// Destructor
Socket::~Socket() {
    if (socket_fd_ != -1) {
        close(socket_fd_);
    }
}

// Send and receive methods
ssize_t Socket::send(int fd, const void* buffer, size_t length) {
    return ::send(fd, buffer, length, 0);
}

ssize_t Socket::send(int fd, const void* buffer, size_t length, std::chrono::milliseconds timeout) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, nullptr, &writefds, nullptr, &tv) > 0) {
        return ::send(fd, buffer, length, 0);
    }
    return -1;  // Timeout or error
}

ssize_t Socket::sendn(int fd, const void* buffer, size_t n) {
    size_t total_sent = 0;
    const char* buf = static_cast<const char*>(buffer);

    while (total_sent < n) {
        ssize_t bytes_sent = ::send(fd, buf + total_sent, n - total_sent, 0);
        if (bytes_sent <= 0) return bytes_sent;
        total_sent += bytes_sent;
    }
    return total_sent;
}

ssize_t Socket::sendn(int fd, const void* buffer, size_t n, std::chrono::milliseconds timeout) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, nullptr, &writefds, nullptr, &tv) > 0) {
        return sendn(fd, buffer, n);
    }
    return -1;
}

ssize_t Socket::sendv(int fd, const struct iovec* vector_buf, int n) {
    return ::writev(fd, vector_buf, n);
}

ssize_t Socket::sendv(int fd, const struct iovec* vector_buf, int n, std::chrono::milliseconds timeout) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, nullptr, &writefds, nullptr, &tv) > 0) {
        return ::writev(fd, vector_buf, n);
    }
    return -1;
}

ssize_t Socket::recv(int fd, void* buffer, size_t length) {
    return ::recv(fd, buffer, length, 0);
}

ssize_t Socket::recv(int fd, void* buffer, size_t length, std::chrono::milliseconds timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
        return ::recv(fd, buffer, length, 0);
    }
    return -1;  // Timeout or error
}

ssize_t Socket::recvn(int fd, void* buffer, size_t n) {
    size_t total_received = 0;
    char* buf = static_cast<char*>(buffer);

    while (total_received < n) {
        ssize_t bytes_received = ::recv(fd, buf + total_received, n - total_received, 0);
        if (bytes_received <= 0) return bytes_received;
        total_received += bytes_received;
    }
    return total_received;
}

ssize_t Socket::recvn(int fd, void* buffer, size_t n, std::chrono::milliseconds timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
        return recvn(fd, buffer, n);
    }
    return -1;
}

ssize_t Socket::recv_vec(int fd, struct iovec* vector_buf, int n) {
    return ::readv(fd, vector_buf, n);
}

ssize_t Socket::recv_vec(int fd, struct iovec* vector_buf, int n, std::chrono::milliseconds timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
        return ::readv(fd, vector_buf, n);
    }
    return -1;
}
