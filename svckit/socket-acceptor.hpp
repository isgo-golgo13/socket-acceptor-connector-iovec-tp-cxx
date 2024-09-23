#pragma once

#include "socket-addr.hpp"
#include "socket.hpp"
#include <sys/socket.h>
#include <netinet/in.h>

class SocketAcceptor {
public:
    explicit SocketAcceptor(const SocketAddr& addr);
    SocketAcceptor() = delete;
    SocketAcceptor(const SocketAcceptor& other) = delete;
    SocketAcceptor(SocketAcceptor&& other) noexcept = default;
    SocketAcceptor& operator=(const SocketAcceptor& other) = delete;
    SocketAcceptor& operator=(SocketAcceptor&& other) noexcept = default;
    ~SocketAcceptor();

    void bind();
    void listen();
    int acceptConnection();
    void sendData(int clientSocket, const void* buffer, size_t length);
    void recvData(int clientSocket, void* buffer, size_t length);

private:
    int serverSocket_;
    sockaddr_in serverAddr_;
    std::unique_ptr<Socket> socket_;
};
