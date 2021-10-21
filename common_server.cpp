#include "common_server.h"
#include <string>
#include <iostream>
#include "common_protocol.h"

Server::Server(): queues() {}

void Server::execute(const char * port) {
    Socket socket;
    socket.bindAndListen(port, 8);
    Socket peer = socket.accept();
    Protocol protocol(peer);

    while(true)
        protocol.receive(*this);


}

void Server::popMessage(Protocol & protocol, const std::string &queue_name) {
    // TODO que hago si no existe la cola?
    try {
        protocol.sendMessage(queues.at(queue_name).pop());
    }
    catch (const std::out_of_range & e) {
        // TODO PASAR ESTO AL HILO DEL HILO
        protocol.sendMessage("");
        std::cerr << "Se intento popear de una cola inexistente: '" << queue_name << "'" << std::endl;
    }
}

void Server::pushMessage(const std::string &queue_name, const std::string &message) {
    // TODO que hago si no existe esa cola?
    try {
        queues.at(queue_name).push(message);
    }
    catch (const std::out_of_range & e) {
        // TODO PASAR ESTO AL HILO DEL HILO
        std::cerr << "Se intento pushear en una cola inexistente: '" << queue_name << "' el mensaje '" << message << "'" << std::endl;
    }
}

void Server::defineQueue(const std::string &queue_name) {
    BlockingQueue<std::string> new_queue;
    queues.insert(std::pair<std::string, BlockingQueue<std::string>>(queue_name, std::move(new_queue)));
}

