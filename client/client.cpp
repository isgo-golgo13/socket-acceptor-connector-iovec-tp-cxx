#include "socket-connector.hpp"
#include "socket-addr.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <sys/uio.h>

constexpr int SESSION_COUNT = 6;  // Number of sessions
constexpr int PORT = 8080;

int main() {
    // Prepare iovec structs
    std::vector<std::shared_ptr<struct iovec>> iovecs;
    const char* message1 = "Message Part 1\n";
    const char* message2 = "Message Part 2\n";
    const char* message3 = "Message Part 3\n";

    for (int i = 0; i < SESSION_COUNT; i++) {
        auto iov = std::make_shared<struct iovec>();
        if (i % 3 == 0) {
            iov->iov_base = (void*)message1;
            iov->iov_len = strlen(message1);
        } else if (i % 3 == 1) {
            iov->iov_base = (void*)message2;
            iov->iov_len = strlen(message2);
        } else {
            iov->iov_base = (void*)message3;
            iov->iov_len = strlen(message3);
        }
        iovecs.push_back(iov);
    }

    // Create and connect SocketConnector
    SocketAddr addr("127.0.0.1", PORT);
    SocketConnector connector(addr);
    connector.connect();

    // Send data with sendv
    connector.socket_->iovec_vector_ = iovecs;
    connector.sendDataVec();

    return 0;
}
