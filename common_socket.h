#ifndef TP3_TALLER1_SOCKET_H
#define TP3_TALLER1_SOCKET_H

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include "common_packet.h"

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

    Socket accept() const;

    size_t send(Packet & packet) const;

    size_t receive(Packet & packet, size_t size);

    void close();
};


#endif //TP3_TALLER1_SOCKET_H
