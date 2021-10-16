#ifndef TP3_TALLER1_SOCKET_H
#define TP3_TALLER1_SOCKET_H

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include "common_message.h"

#define INVALID_FILE_DESCRIPTOR -1
#define RECV_BUFFER_SIZE 30

class Socket {
    int fd;

    void getAddressInfo(struct addrinfo ** result, const char * host,
                                const char * port);

    explicit Socket(int fd);

public:
    Socket();

    Socket(const Socket & orig) = delete;

    Socket(Socket && orig) noexcept;

    Socket & operator=(const Socket & orig) = delete;

    Socket & operator=(Socket && orig);

    ~Socket();

    void connect(const char * host, const char * port);

    void bindAndListen(const char *port, uint8_t pend_conn);

    Socket accept();

    size_t send(Message &msg) const;

    size_t receive(Message &msg, size_t size) const;
};


#endif //TP3_TALLER1_SOCKET_H
