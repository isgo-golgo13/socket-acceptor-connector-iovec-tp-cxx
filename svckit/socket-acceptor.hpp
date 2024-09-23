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
    void sendData(int clientSocket, const std::vector<char>& buffer);
    void recvData(int clientSocket, std::vector<char>& buffer);

private:
    int serverSocket_;
    sockaddr_in serverAddr_;
    std::unique_ptr<Socket> socket_;
};
