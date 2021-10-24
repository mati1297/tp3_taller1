#include <iostream>
#include <thread>
#include <string>
#include <utility>
#include "server_acceptor_thread.h"
#include "common_socket_closed.h"

AcceptorThread::AcceptorThread(const Socket &socket_, ProtectedMap<std::string,
        BlockingQueue<std::string>> & queues_): socket(socket_), clients(),
                                                threads(), queues(queues_) {}


void AcceptorThread::operator()() {
    // TODO encapsular todo esto y meterlo en un try catch
    // TODO el socket me lo pueden cerrar, debería protegerlo con un mutex



    // TODO validar si la lista esta vacia
    // TODO fijarse que se rompe el q cuando tengo dos abiertos
    // TODO ver bien como es el tema del iterador con el end y eso.
    //  Ver foreach.!!
    try {
        while (true) {
            Socket peer = std::move(socket.accept());

            clients.insert(clients.end(),
                           std::move(ClientThread(std::move(peer), queues)));
            if (!clients.empty())
                threads.insert(threads.end(),
                            std::thread(std::ref(clients.back())));
            else
                throw std::runtime_error("Error al cargar el cliente");

            if (threads.empty())
                throw std::runtime_error("Error al iniciar el hilo");

            auto iterclients = clients.begin();
            auto iterthreads = threads.begin();
            for (; iterclients != clients.end() && iterthreads != threads.end();
                 iterclients++, iterthreads++) {
                if (iterclients->isDead()) {
                    if (iterthreads->joinable())
                        iterthreads->join();
                    iterthreads = threads.erase(iterthreads);
                    iterclients = clients.erase(iterclients);
                }
            }
        }
    }
    catch(const SocketClosed & e){
        auto iterclients = clients.begin();
        auto iterthreads = threads.begin();
        for (; iterclients != clients.end() && iterthreads != threads.end();
             iterclients++, iterthreads++){
            iterclients->stop();
            while (!iterclients->isDead()) {}
            if (iterthreads->joinable())
                iterthreads->join();
        }
    }
    catch(const std::exception & e) {
        auto iterclients = clients.begin();
        auto iterthreads = threads.begin();
        for (; iterclients != clients.end() && iterthreads != threads.end();
             iterclients++, iterthreads++){
            iterclients->stop();
            while (!iterclients->isDead()) {}
            if (iterthreads->joinable())
                iterthreads->join();
        }
    }
}
