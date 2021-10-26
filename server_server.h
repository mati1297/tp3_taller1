#ifndef TP3_TALLER1_SERVER_SERVER_H
#define TP3_TALLER1_SERVER_SERVER_H


#include <string>
#include <map>
#include "server_blocking_queue.h"
#include "common_protocol.h"
#include "server_protected_map.h"

class Server {
    // TODO Hacerle monitor a esto.
    ProtectedMap<std::string, std::string> queues;


public:
    Server();

    void execute(const char * port);

    // TODO despues esto debe recibir el protocolo.
};


#endif //TP3_TALLER1_SERVER_SERVER_H
