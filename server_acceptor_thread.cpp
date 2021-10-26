#include <iostream>
#include <thread>
#include <string>
#include <utility>
#include "server_acceptor_thread.h"
#include "common_socket_closed.h"

AcceptorThread::AcceptorThread(const Socket &socket_, ProtectedBlockingQueueMap<std::string,
        std::string> & queues_): socket(socket_), clients(), queues(queues_),
        thread() {
    // Arranca el hilo.
    thread = std::thread(std::ref(*this));
}

void AcceptorThread::operator()() {
    try {
        // Se itera siempre que no se cierre el socket aceptador.
        while (true) {
            // Se acepta un cliente.
            Socket peer = socket.accept();

            // Se crea un nuevo thread de cliente y se agrega a la lista.
            clients.insert(clients.end(),
                           std::move(ClientThread(std::move(peer), queues)));
            // Se pone a correr el ultimo cliente agregado.
            clients.back().run();

            checkDeadClients();
        }
    }

    // Si se salio del ciclo porque se cerro el socket no se hace nada.
    catch(const SocketClosed & e) {}
    // Si no se imprime un error.
    catch(const std::exception & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    stopClients();
}

void AcceptorThread::stopClients() {
    // Se terminan todos los hilos de clientes activos.
    auto it = clients.begin();
    for (; it != clients.end(); ++it){
        it->stop();
        if (it->joinable())
            it->join();
    }
}

void AcceptorThread::checkDeadClients(){
    /* Se itera los clientes para ver si alguno esta inactivo,
     * si es asi se borra. */
    auto it = clients.begin();
    for (; it != clients.end();){
        if (it->isDead()) {
            if (it->joinable())
                it->join();
            it = clients.erase(it);
        } else {
            ++it;
        }
    }
}

void AcceptorThread::join() {
    thread.join();
}

bool AcceptorThread::joinable() const {
    return thread.joinable();
}
