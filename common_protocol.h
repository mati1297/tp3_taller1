#ifndef TP3_TALLER1_COMMON_PROTOCOL_H
#define TP3_TALLER1_COMMON_PROTOCOL_H

#include <string>
#include "common_socket.h"

#define DEFINE_QUEUE_CHAR 'd'
#define PUSH_CHAR 'u'
#define POP_CHAR 'o'

/* Clase que se encarga de enviar y recibir
 * datos entre cliente y servidor. Posee
 * como atributo una referencia al socket
 * a traves del cual realiza la comunicacion. */
class Protocol {
    Socket & socket;

    void packText(Packet & packet, const std::string &message) const;

public:
    typedef enum {
        DEFINE_QUEUE,
        PUSH,
        POP,
        NO_CMD
    } Command;

    explicit Protocol(Socket & socket_);

    Protocol(const Protocol &) = delete;

    Protocol(const Protocol &&) = delete;

    Protocol & operator=(const Protocol &) = delete;

    Protocol & operator=(const Protocol &&) = delete;

    // Envia un comando para definir la cola queue_name.
    void sendDefineQueue(const std::string & queue_name) const;

    // Envia un comando para pushear un message a la cola queue_name.
    void sendPushMessage(const std::string &queue_name,
                         const std::string &message) const;

    // Envia un comando para popear de la cola queue_name.
    void sendPopMessage(const std::string &queue_name) const;

    // Recibe un mensaje y carga los datos en queue_name y message si los hay.
    Command receive(std::string & queue_name, std::string & message) const;

    // Envia un mensaje message.
    void sendMessage(const std::string & message) const;

    // Recibe texto, lo desempaca y lo devuelve.
    std::string receiveAndUnpackText() const;
};


#endif //TP3_TALLER1_COMMON_PROTOCOL_H
