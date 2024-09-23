#include "socket-acceptor.hpp"
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

SocketAcceptor::SocketAcceptor(const SocketAddr& addr) : serverSocket_(socket(AF_INET, SOCK_STREAM, 0)), socket_(std::make_unique<Socket>(serverSocket_)) {
    if (serverSocket_ == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = inet_addr(addr.getIp().c_str());
    serverAddr_.sin_port = htons(addr.getPort());
}

SocketAcceptor::~SocketAcceptor() {
    close(serverSocket_);
}

void SocketAcceptor::bind() {
    if (::bind(serverSocket_, (sockaddr*)&serverAddr_, sizeof(serverAddr_)) < 0) {
        std::cerr << "Bind failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SocketAcceptor::listen() {
    if (::listen(serverSocket_, 10) < 0) {
        std::cerr << "Listen failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

int SocketAcceptor::acceptConnection() {
    int clientSocket = accept(serverSocket_, nullptr, nullptr);
    if (clientSocket < 0) {
        std::cerr << "Accept failed!" << std::endl;
        return -1;
    }
    return clientSocket;
}

void SocketAcceptor::sendData(int clientSocket, const void* buffer, size_t length) {
    socket_->send(clientSocket, buffer, length);
}

void SocketAcceptor::recvData(int clientSocket, void* buffer, size_t length) {
    socket_->recv(clientSocket, buffer, length);
}
