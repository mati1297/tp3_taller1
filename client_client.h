#ifndef TP3_TALLER1_CLIENT_H
#define TP3_TALLER1_CLIENT_H


#include <map>
#include "common_socket.h"
#include "common_protocol.h"



class Client {
private:
    class Command{
    public:
        virtual void operator()(const Client & client, const std::string &queue_name, const std::string &message) const = 0;
    };

    class CommandSendPopMessage: public Command {
    public:
        void operator()(const Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    class CommandSendPushMessage: public Command {
    public:
        void operator()(const Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    class CommandDefineQueueMessage: public Command {
    public:
        void operator()(const Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    Socket socket;
    Protocol protocol;
    const CommandSendPopMessage send_pop;
    const CommandSendPushMessage send_push;
    const CommandDefineQueueMessage send_define_queue;
    const std::map<const std::string, const Command* const> functors;

public:
    Client(const char * host, const char * port);

    void execute(const char * host, const char * port);

};




#endif //TP3_TALLER1_CLIENT_H
