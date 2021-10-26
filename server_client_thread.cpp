#include <iostream>
#include "server_client_thread.h"
#include "common_protocol.h"
#include "server_protected_blocking_queue_map.h"
#include "common_socket_closed.h"

#include <string>
#include <utility>

// TODO REVISAR CONST EN TODOS LADOS.

ClientThread::ClientThread(Socket && peer_, ProtectedBlockingQueueMap<std::string,
                           std::string> & queues_):
                           keep_talking(false), is_running(false),
                           peer(std::move(peer_)), protocol(peer),
                           queues(queues_), thread() {}

// TODO revisar safety

ClientThread::ClientThread(ClientThread &&orig) noexcept:
                           keep_talking(false), is_running(false),
                           peer(std::move(orig.peer)), protocol(peer),
                           queues(orig.queues), thread() {
    // Si habia un hilo corriendo en el original, se detiene y se joinea.
    orig.stop();
    if (orig.joinable())
        orig.join();
}

void ClientThread::run(){
    // Si esta detenido se lanza el thread.
    if (!is_running) {
        thread = std::thread(std::ref(*this));
        keep_talking = true;
        is_running = true;
    }
}

void ClientThread::operator()() {
    try {
        // Mientras que se pueda seguir hablando se itera.
        while (keep_talking) {
            std::string queue_name, message;
            Protocol::Command cmd;
            /* Se recibe el comando y el nombre de la cola y mensaje
             * (si existe). */
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
    // Si se salio del ciclo porque se cerro el socket no se hace nada.
    catch(const SocketClosed & e) {}
    // Si ocurrio un status se imprime el mensaje.
    catch(const std::exception & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    catch(...) {
        std::cerr << "Error desconocido" << std::endl;
    }
    // Ya no se corre mas.
    is_running = false;
}

bool ClientThread::isDead() {
    return !is_running;
}

void ClientThread::stop() {
    // Se indica que se deje de hablar y se cierra el socket.
    keep_talking = false;
    peer.shutdownAndClose();
}

void ClientThread::popMessage(const std::string &queue_name) {
    try {
        protocol.sendMessage(queues.at(queue_name).pop());
    }
    catch(const std::out_of_range & e) {
        /* Si no existe la cola se envia un mensaje vacio para destrabar
         * el cliente. */
        protocol.sendMessage("");
        std::cerr << "Se intento popear de una cola inexistente: '"
                  << queue_name << "'" << std::endl;
    }
}

void ClientThread::pushMessage(const std::string &queue_name,
                               const std::string &message) {
    try {
        queues.at(queue_name).push(message);
    }
    catch(const std::out_of_range & e) {
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

void ClientThread::join() {
    thread.join();
}

bool ClientThread::joinable() {
    return thread.joinable();
}



