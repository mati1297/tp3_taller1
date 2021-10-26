#ifndef TP3_TALLER1_SERVER_SERVER_H
#define TP3_TALLER1_SERVER_SERVER_H


#include <string>
#include <map>
#include "server_blocking_queue.h"
#include "common_protocol.h"
#include "server_protected_blocking_queue_map.h"

#define EXIT_CMD "q"
#define PEND_CONNECTIONS 8

/* Clase que ejecuta el server. Posee como atributo
 * un map protegido de colas de string. */
class Server {
    ProtectedBlockingQueueMap<std::string, std::string> queues;
    Socket acceptor_socket;

public:
    /* Constructor.
     * Pre:
     *  -port debe apuntar a una direccion de memoria valida. */
    Server(const char * port);

    Server(const Server &r) = delete;

    Server(Server &&) = delete;

    void operator=(const Server &) = delete;

    void operator=(Server &&) = delete;

    // Ejecuta el server en el puerto indicado.
    void execute();
};


#endif //TP3_TALLER1_SERVER_SERVER_H
