#include <string>
#include <arpa/inet.h>
#include <iostream>
#include "common_protocol.h"
#include "common_socket.h"
#include "common_server.h"
#include "common_packet.h"

Protocol::Protocol(const Socket & socket_): socket(socket_) {}

void Protocol::sendDefineQueue(const std::string & queue_name) const{
    Packet packet;
    packet.addByte(DEFINE_QUEUE_CHAR);
    packText(packet, queue_name);

    socket.send(packet);
}

void Protocol::sendPushMessage(const std::string & queue_name, const std::string & message) const {
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

void Protocol::receive(Server & server) {
    Packet packet;

    socket.receive(packet, 1);

    switch(packet.getByte()) {
        case DEFINE_QUEUE_CHAR:
            receiveDefineQueue(server, packet);
            break;
        case PUSH_CHAR:
            receivePushMessage(server, packet);
            break;
        case POP_CHAR:
            receivePopMessage(server, packet);
            break;
        default:
            break;
    }
}

void Protocol::receiveDefineQueue(Server & server, Packet & packet) {
    std::string queue_name;

    queue_name = receiveAndUnpackText();

    server.defineQueue(queue_name);
}

void Protocol::receivePushMessage(Server & server, Packet & packet){
    std::string queue_name, message;

    queue_name = receiveAndUnpackText();
    std::cout << queue_name << std::endl;
    message = receiveAndUnpackText();

    server.pushMessage(queue_name, message);
}

void Protocol::receivePopMessage(Server & server, Packet & packet){
    std::string queue_name;

    queue_name = receiveAndUnpackText();

    server.popMessage(*this, queue_name);
}

void Protocol::sendMessage(const std::string & message) {
    Packet packet;
    packText(packet, message);
    socket.send(packet);
}

void Protocol::packText(Packet & packet, const std::string & text) const {
    if(text.size() > UINT16_MAX)
        throw std::invalid_argument("el texto a enviar es demasiado largo, el largo maximo es de " + std::to_string(UINT16_MAX));
    packet.addBytes(htons(text.size()));
    packet.addBytes(text);
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
