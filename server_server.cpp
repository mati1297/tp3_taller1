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

    while (true) {
        std::string input;
        std::cin >> input;
        if (input != EXIT_CMD)
            std::cerr << "Incorrecto. Para cerrar ingresa "
                      << EXIT_CMD << std::endl;
        else
            break;
    }
    socket.shutdownAndClose();
    queues.unlockAll();
    if (acceptor_thread.joinable())
        acceptor_thread.join();
}



