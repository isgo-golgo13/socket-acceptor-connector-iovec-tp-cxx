# Socket Acceptor-Connector using POSIX I/O Vectors
C++20 POSA 2 Socket Acceptor-Connector Design Pattern for Client and Server using POSIX I/O Vectors with Thread Pool (TP) from ACE C++ Framework



## Compiling the Project

```shell
cmake -S . -B ./dist 
```

Next produce the exes.

```shell
cmake --build ./dist
```

This root-level CMakeLists.txt will call on the `client/CMakeLists.txt` and `server/CMakeLists.txt` files that in-turn will require to create the `svckit` Unix shared library.


## Project Structure

The project structure is as follows.

```shell
├── CMakeLists.txt
├── Dockerfile.client
├── Dockerfile.server
├── README.md
├── client
│   ├── CMakeLists.txt
│   └── client.cpp
├── docker-compose.yaml
├── server
│   ├── CMakeLists.txt
│   └── server.cpp
└── svckit
    ├── CMakeLists.txt
    ├── socket.cpp
    ├── socket.hpp
    ├── socket-acceptor.cpp
    ├── socket-acceptor.hpp
    ├── socket-addr.cpp
    ├── socket-addr.hpp
    ├── socket-connector.cpp
    ├── socket-connector.hpp
    ├── thread-pool.cpp
    ├── thread-pool.hpp
    ├── thread.cpp
    └── thread.hpp
```



## Running the Client and Server in Docker

Provided in the project is the `docker-compose.yaml` file.
The socket client and socket server run over a `thread_pool_network` Docker network bridge.


```yaml
version: '3.8'

services:
  server:
    build:
      context: .
      dockerfile: Dockerfile.server
    container_name: thread_pool_server
    ports:
      - "8080:8080"  # Expose server port
    networks:
      - thread_pool_network

  client:
    build:
      context: .
      dockerfile: Dockerfile.client
    container_name: thread_pool_client
    depends_on:
      - server  # Ensure the server starts before the client
    networks:
      - thread_pool_network
    environment:
      SERVER_HOST: "server"  # The server's hostname
      SERVER_PORT: "8080"    # Port to connect to

networks:
  thread_pool_network:
    driver: bridge
```

To run the client and server do the following.

```shell
docker-compose --build up
```

To stop the client and server do the following.

```shell
docker-compose down
```

