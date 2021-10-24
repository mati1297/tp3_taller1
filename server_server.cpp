#include "server_server.h"
#include <string>
#include <thread>
#include <iostream>
#include "common_protocol.h"
#include "server_acceptor_thread.h"

Server::Server(): queues() {}

void Server::execute(const char * port) {
    Socket socket;
    socket.bindAndListen(port, 8);

    AcceptorThread acceptor_thread(socket, queues);

    std::thread thread(std::ref(acceptor_thread));

    std::string input;

    // TODO Refactor (aca o en el otro)
    while (true) {
        std::cin >> input;
        if (input == "q")
            break;
    }

    socket.close();
    thread.join();
}



