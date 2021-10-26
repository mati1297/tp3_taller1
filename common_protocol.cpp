#include <string>
#include <arpa/inet.h>
#include <iostream>
#include "common_protocol.h"
#include "common_socket.h"
#include "common_packet.h"

Protocol::Protocol(Socket & socket_): socket(socket_) {}

void Protocol::sendDefineQueue(const std::string & queue_name) const{
    Packet packet;
    packet.addByte(DEFINE_QUEUE_CHAR);
    packText(packet, queue_name);

    socket.send(packet);
}

void Protocol::sendPushMessage(const std::string & queue_name,
                               const std::string & message) const {
    Packet packet;
    packet.addByte(PUSH_CHAR);
    packText(packet, queue_name);
    packText(packet, message);

    socket.send(packet);
}

void Protocol::sendPopMessage(const std::string & queue_name) const {
    Packet packet;
    packet.addByte(POP_CHAR);
    packText(packet, queue_name);

    socket.send(packet);
}

Protocol::Command Protocol::receive(std::string & queue_name,
                                    std::string & message) const {
    Packet packet;

    socket.receive(packet, 1);

    switch (packet.getByte()) {
        case DEFINE_QUEUE_CHAR:
            queue_name = receiveAndUnpackText();
            return DEFINE_QUEUE;
        case PUSH_CHAR:
            queue_name = receiveAndUnpackText();
            message = receiveAndUnpackText();
            return PUSH;
        case POP_CHAR:
            queue_name = receiveAndUnpackText();
            return POP;
        default:
            queue_name = "";
            return NO_CMD;
    }
}

void Protocol::sendMessage(const std::string & message) const {
    Packet packet;
    packText(packet, message);
    socket.send(packet);
}

std::string Protocol::receiveAndUnpackText() const {
    Packet packet;
    uint16_t size = 0;
    socket.receive(packet, 2);
    packet.getBytes(size);
    size = ntohs(size);
    socket.receive(packet, size);
    std::string text;
    packet.getBytes(text, size);
    return text;
}

void Protocol::packText(Packet & packet, const std::string & text) const {
    if (text.size() > UINT16_MAX)
        throw std::invalid_argument("el texto a enviar es demasiado largo, "
                                    "el largo maximo es de "
                                    + std::to_string(UINT16_MAX));
    packet.addBytes(htons(text.size()));
    packet.addBytes(text);
}


