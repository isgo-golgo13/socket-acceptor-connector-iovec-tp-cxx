#include "socket-connector.hpp"
#include "socket-addr.hpp"
#include <iostream>
#include <memory>
#include <vector>
#include <string_view>
#include <sys/uio.h>

constexpr int SESSION_COUNT = 6;
constexpr int PORT = 8080;

int main() {
    // Prepare iovec structs using std::string_view for payloads
    std::vector<std::shared_ptr<struct iovec>> iovecs;
    std::string_view payload1 = "Payload Part 1\n";
    std::string_view payload2 = "Payload Part 2\n";
    std::string_view payload3 = "Payload Part 3\n";

    for (int i = 0; i < SESSION_COUNT; ++i) {
        auto iov = std::make_shared<struct iovec>();
        if (i % 3 == 0) {
            iov->iov_base = const_cast<char*>(payload1.data());
            iov->iov_len = payload1.size();
        } else if (i % 3 == 1) {
            iov->iov_base = const_cast<char*>(payload2.data());
            iov->iov_len = payload2.size();
        } else {
            iov->iov_base = const_cast<char*>(payload3.data());
            iov->iov_len = payload3.size();
        }
        iovecs.push_back(iov);
    }

    // Create and connect SocketConnector
    SocketAddr addr("127.0.0.1", PORT);
    SocketConnector connector(addr);
    connector.connect();

    // Send data with sendv
    connector.sendDataVec(iovecs);

    return 0;
}
