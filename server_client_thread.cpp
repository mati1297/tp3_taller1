#include <iostream>
#include "server_client_thread.h"
#include "common_protocol.h"
#include "server_protected_map.h"
#include "common_socket_closed.h"

#include <string>
#include <utility>

// TODO REVISAR CONST EN TODOS LADOS.

ClientThread::ClientThread(Socket && peer_, ProtectedMap<std::string,
                           BlockingQueue<std::string>> & queues_):
                           keep_talking(true), is_running(true),
                           peer(std::move(peer_)), protocol(peer),
                           queues(queues_) {}

// TODO revisar safety

ClientThread::ClientThread(ClientThread &&orig) noexcept:
                           keep_talking(true), is_running(true),
                           peer(std::move(orig.peer)), protocol(peer),
                           queues(orig.queues) {
    if (orig.keep_talking)
        keep_talking = true;
    else
        keep_talking = false;
    if (orig.is_running)
        is_running = true;
    else
        is_running = false;
    orig.keep_talking = false;
    orig.is_running = false;
}


void ClientThread::operator()() {
    try {
        while (keep_talking) {
            std::string queue_name, message;
            Protocol::Command cmd;
            cmd = protocol.receive(queue_name, message);
            switch (cmd) {
                case Protocol::DEFINE_QUEUE:
                    defineQueue(queue_name);
                    break;
                case Protocol::PUSH:
                    pushMessage(queue_name, message);
                    break;
                case Protocol::POP:
                    popMessage(queue_name);
                    break;
                case Protocol::NO_CMD:
                    break;
            }
        }
    }
    catch(const SocketClosed & e) {
        // TODO dar algun mensaje aca?
    }
    catch(const std::exception & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    is_running = false;
    peer.close();
}

// TODO TENGO QUE DESTRUIR LOS CLIENTES.

bool ClientThread::isDead() {
    return !is_running;
}

void ClientThread::stop() {
    keep_talking = false;
    peer.close();
}

void ClientThread::popMessage(const std::string &queue_name) {
    // TODO que hago si no existe la cola?
    try {
        protocol.sendMessage(queues.at(queue_name).pop());
    }
    catch(const std::out_of_range & e) {
        // TODO PASAR ESTO AL HILO DEL HILO
        protocol.sendMessage("");
        std::cerr << "Se intento popear de una cola inexistente: '"
                  << queue_name << "'" << std::endl;
    }
}

void ClientThread::pushMessage(const std::string &queue_name,
                               const std::string &message) {
    // TODO que hago si no existe esa cola?
    try {
        queues.at(queue_name).push(message);
    }
    catch(const std::out_of_range & e) {
        // TODO PASAR ESTO AL HILO DEL HILO
        std::cerr << "Se intento pushear en una cola inexistente: '"
                  << queue_name << "' el mensaje '"
                  << message << "'" << std::endl;
    }
}

void ClientThread::defineQueue(const std::string &queue_name) {
    BlockingQueue<std::string> new_queue;
    queues.insert(std::pair<std::string, BlockingQueue<std::string>>
                                        (queue_name, std::move(new_queue)));
}




