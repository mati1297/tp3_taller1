#ifndef TP3_TALLER1_CLIENT_H
#define TP3_TALLER1_CLIENT_H


#include <map>
#include "common_socket.h"
#include "common_protocol.h"



class Client {
private:
    class Command{
    public:
        virtual void operator()(Client & client, const std::string &queue_name, const std::string &message) const = 0;
    };

    class CommandSendPopMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    class CommandSendPushMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    class CommandDefineQueueMessage: public Command {
    public:
        void operator()(Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    class CommandExit: public Command {
        void operator()(Client & client, const std::string &queue_name, const std::string &message) const override;
    };

    Socket socket;
    Protocol protocol;
    const CommandSendPopMessage cmd_send_pop;
    const CommandSendPushMessage cmd_send_push;
    const CommandDefineQueueMessage cmd_send_define_queue;
    const CommandExit cmd_exit;
    const std::map<const std::string, const Command* const> functors;
    bool exit_flag;

public:
    Client(const char * host, const char * port);

    void execute();

    void setExit();

};




#endif //TP3_TALLER1_CLIENT_H
