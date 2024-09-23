#pragma once

#include <string>
#include <arpa/inet.h>

class SocketAddr {
public:
    SocketAddr() = default;
    SocketAddr(const std::string& ip, int port);
    SocketAddr(const SocketAddr& other) = default;
    SocketAddr(SocketAddr&& other) noexcept = default;
    SocketAddr& operator=(const SocketAddr& other) = default;
    SocketAddr& operator=(SocketAddr&& other) noexcept = default;
    ~SocketAddr() = default;

    [[nodiscard]] std::string getIp() const;
    [[nodiscard]] int getPort() const;

private:
    //struct sockaddr_in addr_;  //SocketAddr wraps sockaddr_in struct 
    std::string ip_;
    int port_;
};