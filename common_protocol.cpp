#include <string>
#include <arpa/inet.h>
#include <iostream>
#include "common_protocol.h"
#include "common_socket.h"
#include "common_message.h"

Protocol::Protocol(const Socket * const socket_): socket(socket_) {}

void Protocol::sendDefineQueue(const std::string & queue_name) {
    Message msg;
    msg.addByte(DEFINE_QUEUE_CHAR);
    //TODO validar tamanio
    msg.addBytes(htons(queue_name.size()));
    msg.addBytes(queue_name);
    msg.print();
    try{
        socket->send(msg);
    }
    catch(std::exception & e) {
        throw;
    }
}

void Protocol::sendPush(const std::string & queue_name, const std::string & message) {
    Message msg;
    msg.addByte(PUSH_CHAR);
    // TODO validar tamanio
    msg.addBytes(htons(queue_name.size()));
    msg.addBytes(queue_name);
    msg.addBytes(htons(message.size()));
    msg.addBytes(message);
    try {
        socket->send(msg);
    }
    catch(std::exception & e){
        throw;
    }
}
