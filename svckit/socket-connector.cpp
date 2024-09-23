#include "socket-connector.hpp"
#include <unistd.h>
#include <iostream>
#include <memory>

SocketConnector::SocketConnector(const SocketAddr& addr) : serverAddr_(addr), clientSocket_(socket(AF_INET, SOCK_STREAM, 0)) {
    if (clientSocket_ == -1) {
        std::cerr << "Socket creation failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
    socket_ = std::make_unique<Socket>(clientSocket_);
}

void SocketConnector::connect() {
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverAddr_.getPort());
    serverAddr.sin_addr.s_addr = inet_addr(serverAddr_.getIp().c_str());

    if (::connect(clientSocket_, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed!" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SocketConnector::sendData(const void* buffer, size_t length) {
    socket_->send(clientSocket_, buffer, length);
}

void SocketConnector::sendDataVec() {
    socket_->sendv(clientSocket_, socket_->iovec_vector_.data(), socket_->iovec_vector_.size());
}
