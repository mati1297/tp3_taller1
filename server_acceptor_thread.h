#ifndef TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H
#define TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H


#include "common_socket.h"
#include <thread>
#include <list>
#include <string>
#include "server_client_thread.h"

// TODO repasar privados y publicos (en todos lados).

/* Clase wrapper del thread aceptador. Posee una referencia al socket
 * aceptador, una lista con los clientes activos, un map con las colas
 * y el thread correspondiente. */
class AcceptorThread {
    const Socket & socket;
    std::list<ClientThread> clients;
    ProtectedMap<std::string, std::string> & queues;
    std::thread thread;

public:
    /* Constructor. Se le pasa una ref al socket y otra al map de
     * colas. */
    AcceptorThread(const Socket & socket, ProtectedMap<std::string,
            std::string> & queues_);

    AcceptorThread(const AcceptorThread &) = delete;

    AcceptorThread(AcceptorThread &&) = default;

    AcceptorThread & operator=(const AcceptorThread &) = delete;

    AcceptorThread & operator=(AcceptorThread &&) = delete;

    // Metodo que ejecuta el thread.
    void operator()();

    // Joinea el hilo.
    void join();

    // Devuelve true si el hilo se puede joinear.
    bool joinable() const;
};


#endif //TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H
