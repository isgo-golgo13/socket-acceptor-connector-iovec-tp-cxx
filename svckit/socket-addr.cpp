#include "socket-addr.hpp"

SocketAddr::SocketAddr(const std::string& ip, int port) : ip_(ip), port_(port) {}

std::string SocketAddr::getIp() const {
    return ip_;
}

int SocketAddr::getPort() const {
    return port_;
}