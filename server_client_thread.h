#ifndef TP3_TALLER1_SERVER_CLIENT_THREAD_H
#define TP3_TALLER1_SERVER_CLIENT_THREAD_H

#include <atomic>
#include <thread>
#include <string>
#include "common_socket.h"
#include "common_protocol.h"
#include "server_blocking_queue.h"
#include "server_protected_map.h"

class ClientThread {
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket peer;
    Protocol protocol;
    ProtectedMap<std::string, BlockingQueue<std::string>> & queues;
/*
    void popMessage(const std::string &queue_name);

    void pushMessage(const std::string &queue_name, const std::string &message);

    void defineQueue(const std::string &queue_name);
*/
public:
    ClientThread(Socket && peer_, ProtectedMap<std::string,
                 BlockingQueue<std::string>> & queues_);

    ClientThread(ClientThread && orig) noexcept;

    ClientThread(const ClientThread &) = delete;

    ClientThread & operator=(const ClientThread &) = delete;

    ClientThread & operator=(ClientThread &&) = delete;

    void operator()();

    /*bool isDead();

    void stop();*/
};


#endif //TP3_TALLER1_SERVER_CLIENT_THREAD_H
