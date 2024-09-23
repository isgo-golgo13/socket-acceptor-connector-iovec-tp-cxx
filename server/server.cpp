#include "socket-acceptor.hpp"
#include "socket-addr.hpp"
#include "thread-pool.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <string>

constexpr int PORT = 8080;
constexpr int BUFFER_SIZE = 1024;
constexpr size_t THREAD_POOL_SIZE = 4;

void handleSession(int clientSocket, SocketAcceptor& acceptor) {
    std::vector<char> buffer(BUFFER_SIZE);
    acceptor.recvData(clientSocket, buffer);
    std::cout << "Received data: " << std::string(buffer.data(), buffer.size()) << std::endl;

    close(clientSocket);
}

int main() {
    SocketAddr addr("0.0.0.0", PORT);
    SocketAcceptor acceptor(addr);
    ThreadPool threadPool(THREAD_POOL_SIZE);

    acceptor.bind();
    acceptor.listen();

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        int clientSocket = acceptor.acceptConnection();
        if (clientSocket < 0) continue;

        threadPool.enqueueTask([clientSocket, &acceptor]() {
            handleSession(clientSocket, acceptor);
        });
    }

    return 0;
}
