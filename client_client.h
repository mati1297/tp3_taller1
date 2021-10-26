#ifndef TP3_TALLER1_CLIENT_H
#define TP3_TALLER1_CLIENT_H

#include <map>
#include <string>
#include "common_socket.h"
#include "common_protocol.h"

/* Clase que ejecuta al cliente. Posee un map con los comandos,
 * instancias a cada uno de los comandos, un socket, un protocolo
 * y un flag de exit. */
class Client {
private:
    /* Clase Command, clase abstracta que contiene un opeador() para ejecutarse,
     * y sus hijas. */
    class Command{
    public:
        virtual void operator()(Client & client, const std::string &queue_name,
                const std::string &message) const = 0;
    };

    class CommandSendPopMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name,
                const std::string &message) const override;
    };

    class CommandSendPushMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name,
                const std::string &message) const override;
    };

    class CommandDefineQueueMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name,
                const std::string &message) const override;
    };

    class CommandExit: public Command {
        void operator()(Client & client, const std::string &queue_name,
                const std::string &message) const override;
    };

    Socket socket;
    Protocol protocol;
    const CommandSendPopMessage cmd_send_pop;
    const CommandSendPushMessage cmd_send_push;
    const CommandDefineQueueMessage cmd_send_define_queue;
    const CommandExit cmd_exit;
    const std::map<const std::string, const Command* const> functors;
    bool exit_flag;

    // Lee una linea por stdin y la devuelve en distintas strings.
    void readLine(std::string &command, std::string &queue_name, std::string &message);

    // Setea el flag de exit.
    void setExit();
public:
    /* Constructor. Se le pasa el host y el port al que se debe
     * conectar el cliente.
     * Pre:
     *  -host y port deben apuntar a posiciones de memoria validas. */
    Client(const char * host, const char * port);

    // Ejecuta el server.
    void execute();
};




#endif //TP3_TALLER1_CLIENT_H
