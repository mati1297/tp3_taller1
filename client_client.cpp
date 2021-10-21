#include "client_client.h"
#include "common_socket.h"
#include "common_protocol.h"
#include <iostream>
#include <sstream>

Client::Client(const char * host, const char * port): socket(), protocol(socket), send_pop(), send_push(),
                                                      send_define_queue(), functors {{"push", &send_push}, {"pop", &send_pop}, {"define", &send_define_queue}} {
    socket.connect(host, port);
}


void Client::execute(const char * host, const char * port) {
    while(true) {
        std::string line;

        std::getline(std::cin, line);

        std::istringstream iss(line);

        std::string command, queue_name, message;

        iss >> command >> queue_name >> message;

        (*functors.at(command))(*this, queue_name, message);
    }
}


void Client::CommandSendPopMessage::operator()(const Client &client, const std::string &queue_name,
                                               const std::string &message) const {
    client.protocol.sendPopMessage(queue_name);
    std::cout << client.protocol.receiveAndUnpackText() << std::endl;
}

void Client::CommandSendPushMessage::operator()(const Client &client, const std::string &queue_name,
                                                const std::string &message) const {
    client.protocol.sendPushMessage(queue_name, message);
}

void Client::CommandDefineQueueMessage::operator()(const Client &client, const std::string &queue_name,
                                                   const std::string &message) const {
    client.protocol.sendDefineQueue(queue_name);
}
