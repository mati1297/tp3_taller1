#ifndef TP3_TALLER1_COMMON_PROTOCOL_H
#define TP3_TALLER1_COMMON_PROTOCOL_H

#include "common_socket.h"

#define DEFINE_QUEUE_CHAR 'd'
#define PUSH_CHAR 'u'

class Protocol {
    const Socket * const socket;

public:
    explicit Protocol(const Socket * socket_);

    void sendDefineQueue(const std::string & queue_name);

    void sendPush(const std::string &queue_name, const std::string &message);
};


#endif //TP3_TALLER1_COMMON_PROTOCOL_H
