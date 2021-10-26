#include "server_server.h"
#include <string>
#include <iostream>
#include "server_acceptor_thread.h"

Server::Server(): queues(), acceptor_socket() {}

void Server::execute(const char * port) {
    // Se bindea y se deja en escucha el socket.
    acceptor_socket.bindAndListen(port, PEND_CONNECTIONS);
    // Se lanza el hilo aceptador.
    AcceptorThread acceptor_thread(acceptor_socket, queues);

    // Mientras que no se ingrese el caracter de exit se queda en loop.
    while (true) {
        std::string input;
        std::cin >> input;
        if (input != EXIT_CMD)
            std::cerr << "Incorrecto. Para cerrar ingresa "
                      << EXIT_CMD << std::endl;
        else
            break;
    }
    // Se cierra el socket aceptador.
    acceptor_socket.shutdownAndClose();
    // Se desbloquea el map de colas.
    queues.unlockAll();
    // Se joinea el hilo aceptador.
    if (acceptor_thread.joinable())
        acceptor_thread.join();
}



