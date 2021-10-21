#ifndef TP3_TALLER1_COMMON_PROTOCOL_H
#define TP3_TALLER1_COMMON_PROTOCOL_H

#include "common_socket.h"
#include "common_server.h"

#define DEFINE_QUEUE_CHAR 'd'
#define PUSH_CHAR 'u'
#define POP_CHAR 'o'

class Server;

class Protocol {
    const Socket & socket;

public:
    explicit Protocol(const Socket & socket_);

    Protocol(const Protocol &) = delete;

    Protocol(const Protocol &&) = delete;

    Protocol & operator=(const Protocol &) = delete;

    Protocol & operator=(const Protocol &&) = delete;

    void sendDefineQueue(const std::string & queue_name) const;

    void sendPushMessage(const std::string &queue_name, const std::string &message) const;

    void sendPopMessage(const std::string &queue_name) const;

    void receive(Server & server);

    void packText(Packet & packet, const std::string &message) const;

    void sendMessage(const std::string & message);

    std::string receiveAndUnpackText() const;

    void receivePopMessage(Server &server, Packet &packet);

    void receiveDefineQueue(Server &server, Packet &packet);

    void receivePushMessage(Server &server, Packet &packet);
};


#endif //TP3_TALLER1_COMMON_PROTOCOL_H
