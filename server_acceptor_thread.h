#ifndef TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H
#define TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H


#include "common_socket.h"
#include <thread>
#include <list>
#include <string>
#include "server_client_thread.h"

// TODO CAMBIAR NOMBREA ESTE Y CLIENTE POR ALGO QUE NO SEA THREAD
class AcceptorThread {
    const Socket & socket;
    std::list<ClientThread> clients;
    std::list<std::thread> threads;
    ProtectedMap<std::string, BlockingQueue<std::string>> & queues;

public:
    AcceptorThread(const Socket & socket, ProtectedMap<std::string,
            BlockingQueue<std::string>> & queues_);

    ~AcceptorThread() = default;

    AcceptorThread(const AcceptorThread &) = delete;

    AcceptorThread(AcceptorThread &&) = default;

    AcceptorThread & operator=(const AcceptorThread &) = delete;

    AcceptorThread & operator=(AcceptorThread &&) = delete;

    void operator()();
};


#endif //TP3_TALLER1_SERVER_ACCEPTOR_THREAD_H
