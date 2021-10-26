#ifndef TP3_TALLER1_SERVER_SERVER_H
#define TP3_TALLER1_SERVER_SERVER_H


#include <string>
#include <map>
#include "server_blocking_queue.h"
#include "common_protocol.h"
#include "server_protected_map.h"

#define EXIT_CMD "q"
#define PEND_CONNECTIONS 8

/* Clase que ejecuta el server. Posee como atributo
 * un map protegido de colas de string. */
class Server {
    ProtectedMap<std::string, std::string> queues;


public:
    // Constructor.
    Server();

    /* Ejecuta el server en el puerto indicado.
     * Pre:
     *  -port debe apuntar a una direccion de memoria valida. */
    void execute(const char * port);
};


#endif //TP3_TALLER1_SERVER_SERVER_H
