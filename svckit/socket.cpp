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

// Send using std::vector<char>
ssize_t Socket::send(int fd, const std::vector<char>& buffer) {
    return ::send(fd, buffer.data(), buffer.size(), 0);
}

// Send with timeout using std::vector<char>
ssize_t Socket::send(int fd, const std::vector<char>& buffer, std::chrono::milliseconds timeout) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, nullptr, &writefds, nullptr, &tv) > 0) {
        return ::send(fd, buffer.data(), buffer.size(), 0);
    }
    return -1;  // Timeout or error
}

// Send multiple bytes using std::vector<char>
ssize_t Socket::sendn(int fd, const std::vector<char>& buffer, size_t n) {
    size_t total_sent = 0;

    while (total_sent < n) {
        ssize_t bytes_sent = ::send(fd, buffer.data() + total_sent, n - total_sent, 0);
        if (bytes_sent <= 0) return bytes_sent;
        total_sent += bytes_sent;
    }
    return total_sent;
}

// Send multiple bytes with timeout using std::vector<char>
ssize_t Socket::sendn(int fd, const std::vector<char>& buffer, size_t n, std::chrono::milliseconds timeout) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(fd, &writefds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, nullptr, &writefds, nullptr, &tv) > 0) {
        return sendn(fd, buffer, n);
    }
    return -1;  // Timeout or error
}

// Send using iovec
ssize_t Socket::sendv(int fd, const struct iovec* vector_buf, int n) {
    return ::writev(fd, vector_buf, n);
}

// Send using iovec with timeout
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
    return -1;  // Timeout or error
}

// Receive using std::vector<char>
ssize_t Socket::recv(int fd, std::vector<char>& buffer) {
    return ::recv(fd, buffer.data(), buffer.size(), 0);
}

// Receive with timeout using std::vector<char>
ssize_t Socket::recv(int fd, std::vector<char>& buffer, std::chrono::milliseconds timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
        return ::recv(fd, buffer.data(), buffer.size(), 0);
    }
    return -1;  // Timeout or error
}

// Receive multiple bytes using std::vector<char>
ssize_t Socket::recvn(int fd, std::vector<char>& buffer, size_t n) {
    size_t total_received = 0;

    while (total_received < n) {
        ssize_t bytes_received = ::recv(fd, buffer.data() + total_received, n - total_received, 0);
        if (bytes_received <= 0) return bytes_received;
        total_received += bytes_received;
    }
    return total_received;
}

// Receive multiple bytes with timeout using std::vector<char>
ssize_t Socket::recvn(int fd, std::vector<char>& buffer, size_t n, std::chrono::milliseconds timeout) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);

    struct timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    if (select(fd + 1, &readfds, nullptr, nullptr, &tv) > 0) {
        return recvn(fd, buffer, n);
    }
    return -1;  // Timeout or error
}

// Receive using iovec
ssize_t Socket::recv_vec(int fd, struct iovec* vector_buf, int n) {
    return ::readv(fd, vector_buf, n);
}

// Receive using iovec with timeout
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
    return -1;  // Timeout or error
}

// Accessor for iovec_vector_
std::vector<std::shared_ptr<struct iovec>>& Socket::get_iovec_vector() {
    return iovec_vector_;
}

const std::vector<std::shared_ptr<struct iovec>>& Socket::get_iovec_vector() const {
    return iovec_vector_;
}
