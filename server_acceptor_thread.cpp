#include <iostream>
#include <thread>
#include <string>
#include <utility>
#include "server_acceptor_thread.h"
#include "common_socket_closed.h"

AcceptorThread::AcceptorThread(Socket &socket_, ProtectedMap<std::string,
        std::string> & queues_): socket(socket_), clients(), threads(),
        queues(queues_), thread() {
    // Arranca el hilo.
    thread = std::thread(std::ref(*this));
}

void AcceptorThread::operator()() {
    // TODO validar si la lista esta vacia
    // TODO ver bien como es el tema del iterador con el end y eso.
    //  Ver foreach.!!
    try {
        while (true) {
            Socket peer = std::move(socket.accept());

            clients.insert(clients.end(),
                           std::move(ClientThread(std::move(peer), queues)));
            clients.back().run();

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
    }
    
    catch(const SocketClosed & e) {}

    catch(const std::exception & e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    auto it = clients.begin();
    for (; it != clients.end(); ++it){
        it->stop();
        if (it->joinable())
            it->join();
    }
}

void AcceptorThread::join() {
    thread.join();
}

bool AcceptorThread::joinable() {
    return thread.joinable();
}
