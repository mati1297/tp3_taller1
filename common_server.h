#ifndef TP3_TALLER1_COMMON_SERVER_H
#define TP3_TALLER1_COMMON_SERVER_H


#include <string>
#include <map>
#include "server_blocking_queue.h"
#include "common_protocol.h"

class Protocol;

class Server {
    // TODO Hacerle monitor a esto.
    std::map<std::string, BlockingQueue<std::string>> queues;


public:
    Server();

    void execute(const char * port);

    // TODO despues esto debe recibir el protocolo.
    void popMessage(Protocol & protocol, const std::string & queue_name);

    void pushMessage(const std::string & queue_name, const std::string & message);

    void defineQueue(const std::string & queue_name);
};


#endif //TP3_TALLER1_COMMON_SERVER_H
