#include "client_client.h"
#include <string>
#include "client_missing_parameter.h"
#include <iostream>
#include <sstream>

Client::Client(const char * host, const char * port):
               socket(), protocol(socket), cmd_send_pop(),
               cmd_send_push(), cmd_send_define_queue(),
               functors {{"push", &cmd_send_push}, {"pop", &cmd_send_pop},
                         {"define", &cmd_send_define_queue},
                         {"exit", &cmd_exit}}, exit_flag(false) {
    //socket.connect(host, port);
}


void Client::execute() {
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        std::istringstream iss(line);
        std::string command, queue_name, message;
        iss >> command >> queue_name >> message;

        std::cout << "soy un cliente arrancando" << std::endl;

        // TODO VALIDAR ESTO LO DE MAP CON UN TRY CATCH?
        try {
            (*functors.at(command))(*this, queue_name, message);
        }
        catch(const MissingParameter & e){
            std::cerr << "Error: " << e.what() << ". Reintenta." << std::endl;
            throw;
        }
        catch(const std::out_of_range & e) {
            std::cerr << "Error: el comando no existe. Reintenta." << std::endl;
            throw;
        }

        if (exit_flag) {
            std::cout << "soy un cliente cerrandose" << std::endl;
            return;
        }
    }
}

void Client::setExit() {
    exit_flag = true;
}

void Client::CommandSendPopMessage::operator()(Client &client,
                    const std::string &queue_name,
                    const std::string &message) const {
    if (queue_name.empty())
        throw MissingParameter("nombre de la cola faltante");
    //client.protocol.sendPopMessage(queue_name);
    //std::cout << client.protocol.receiveAndUnpackText() << std::endl;
}

void Client::CommandSendPushMessage::operator()(Client &client,
        const std::string &queue_name, const std::string &message) const {
    if (queue_name.empty())
        throw MissingParameter("nombre de la cola faltante");
    if (message.empty())
        throw MissingParameter("mensaje faltante");
    //client.protocol.sendPushMessage(queue_name, message);
}

void Client::CommandDefineQueueMessage::operator()(Client &client,
        const std::string &queue_name, const std::string &message) const {
    if (queue_name.empty())
        throw MissingParameter("nombre de la cola faltante");
    //client.protocol.sendDefineQueue(queue_name);
}

void Client::CommandExit::operator()(Client &client,
        const std::string &queue_name, const std::string &message) const {
    client.setExit();
}
