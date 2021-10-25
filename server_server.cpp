#include "server_server.h"
#include <string>
#include <thread>
#include <iostream>
#include "common_protocol.h"
#include "server_acceptor_thread.h"

Server::Server(): queues() {}

void Server::execute(const char * port) {
    std::cout << "creo el socket" << std::endl;
    Socket socket;
    std::cout << "bindeo el socket" << std::endl;
    socket.bindAndListen(port, 1);

    std::cout << "creo el functor aceptador" << std::endl;
    AcceptorThread acceptor_thread(socket, queues);

    std::cout << "lazo el thread" << std::endl;
    std::thread thread(std::ref(acceptor_thread));

    std::string input;

    // TODO Refactor (aca o en el otro)
    while (true) {
        // TODO mostrar mensaje si otra letra se ingresa
        std::cin >> input;
        break;
    }
    std::cout << "lei una " << input << std::endl;

    std::cout << "paso a cerrar" << std::endl;
    socket.close();
    std::cout << "cerre el socket" << std::endl;
    thread.join();
    std::cout << "joinee el thread aceptador" << std::endl;
}



