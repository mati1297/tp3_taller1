#ifndef TP3_TALLER1_SERVER_CLIENT_THREAD_H
#define TP3_TALLER1_SERVER_CLIENT_THREAD_H

#include <atomic>
#include <thread>
#include <string>
#include "common_socket.h"
#include "common_protocol.h"
#include "server_blocking_queue.h"
#include "server_protected_blocking_queue_map.h"

/* Clase wrapper del thread de cliente, ejecuta
 * la comunicacion individual con cada cliente.
 * Posee dos variables booleanas atomicas. Un
 * socket conectado al cliente, un atributo protocolo,
 * un map protegido de colas de string y el propio thread. */
class ClientThread {
    std::atomic<bool> keep_talking;
    std::atomic<bool> is_running;
    Socket peer;
    Protocol protocol;
    ProtectedBlockingQueueMap<std::string, std::string> & queues;
    std::thread thread;

    // Popea un mensaje en la cola queue_name.
    void popMessage(const std::string &queue_name);

    // Pushea message en la cola queue_name.
    void pushMessage(const std::string &queue_name, const std::string &message);

    // Define una cola queue_name.
    void defineQueue(const std::string &queue_name);

public:
    /* Constructor. Se le pasa un Socket por movimiento,
     * y una referencia al map protegido de string. */
    ClientThread(Socket && peer_, ProtectedBlockingQueueMap<std::string,
                 std::string> & queues_);

    /* Constructor por movimiento. Al momento de moverse,
     * se mata el hilo anterior si este funcionara, y se
     * debe iniciar de nuevo el nuevo thread. */
    ClientThread(ClientThread && orig) noexcept;

    ClientThread(const ClientThread &) = delete;

    ClientThread & operator=(const ClientThread &) = delete;

    ClientThread & operator=(ClientThread &&) = delete;

    // Metodo que ejecuta el hilo.
    void operator()();

    // Devuelve true si el hilo esta detenido.
    bool isDead();

    // Para el thread.
    void stop();

    // Joinea el thread.
    void join();

    // Devuelve true si el hilo es joineable.
    bool joinable();

    // Inicia el hilo.
    void run();
};


#endif //TP3_TALLER1_SERVER_CLIENT_THREAD_H
