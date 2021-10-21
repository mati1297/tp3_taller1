#ifndef TP3_TALLER1_COMMON_PROTOCOL_H
#define TP3_TALLER1_COMMON_PROTOCOL_H

#include "common_socket.h"

#define DEFINE_QUEUE_CHAR 'd'
#define PUSH_CHAR 'u'
#define POP_CHAR 'o'

class Protocol {
    const Socket & socket;

public:
    typedef enum {
        DEFINE_QUEUE,
        PUSH,
        POP,
        NO_CMD
    } Command;

    explicit Protocol(const Socket & socket_);

    Protocol(const Protocol &) = delete;

    Protocol(const Protocol &&) = delete;

    Protocol & operator=(const Protocol &) = delete;

    Protocol & operator=(const Protocol &&) = delete;

    void sendDefineQueue(const std::string & queue_name) const;

    void sendPushMessage(const std::string &queue_name, const std::string &message) const;

    void sendPopMessage(const std::string &queue_name) const;

    Command receive(std::string & queue_name, std::string & message);

    void packText(Packet & packet, const std::string &message) const;

    void sendMessage(const std::string & message);

    std::string receiveAndUnpackText() const;
};


#endif //TP3_TALLER1_COMMON_PROTOCOL_H
