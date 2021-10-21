#include "common_socket.h"
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <exception>
#include <stdexcept>
#include "common_packet.h"
#include "socket_closed.h"

// TODO cambiar excepciones que correspondan por excepciones personalizadas

Socket::Socket(): fd(INVALID_FILE_DESCRIPTOR) {}

Socket::Socket(Socket && orig) noexcept: fd(orig.fd) {
    orig.fd = INVALID_FILE_DESCRIPTOR;
}

Socket & Socket::operator=(Socket &&orig) {
    this->fd = orig.fd;
    orig.fd = INVALID_FILE_DESCRIPTOR;
    return *this;
}

Socket::~Socket() {
    if(fd != INVALID_FILE_DESCRIPTOR) {
        ::close(fd);
        fd = INVALID_FILE_DESCRIPTOR;
    }
}

void Socket::connect(const char * host, const char * port) {
    struct addrinfo * result, * ptr;
    getAddressInfo(&result, host, port);

    for(ptr = result; ptr; ptr = ptr->ai_next){
        int skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (skt != -1){
            if (::connect(skt, ptr->ai_addr, ptr->ai_addrlen)) {
                close(skt);
            } else {
                fd = skt;
                break;
            }
        }
    }

    freeaddrinfo(result);
    if(fd == INVALID_FILE_DESCRIPTOR)
        throw std::runtime_error("fallo al conectar al host");

    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
}

void Socket::bindAndListen(const char * port, uint8_t pend_conn) {
    struct addrinfo * result, * ptr;

    getAddressInfo(&result, nullptr, port);

    for(ptr = result; ptr; ptr = ptr->ai_next) {
        int skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(skt != -1){
            int optval = 1;
            if(setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0);
                // TODO validar
            if(::bind(skt, result->ai_addr, result->ai_addrlen)) {
                close(skt);
            } else{
                fd = skt;
                break;
            }
        }
    }

    freeaddrinfo(result);
    if (fd == INVALID_FILE_DESCRIPTOR)
        throw std::runtime_error("fallo al bindear el socket");

    if (listen(fd, pend_conn))
        throw std::runtime_error("fallo al poner el puerto en escucha");
}

Socket Socket::accept() const {
    int fd_peer = ::accept(fd, nullptr, nullptr);
    if (fd_peer == -1)
        throw std::runtime_error("fallo al aceptar a peer");
    Socket peer(fd_peer);
    return peer;
}

void Socket::getAddressInfo(struct addrinfo ** result, const char * host,
                            const char * port) {
    // TODO crear clase para addrinfo?.
    struct addrinfo hints{};
    ::memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (host == nullptr)
        hints.ai_flags = AI_PASSIVE;
    int s = getaddrinfo(host, port, &hints, result);
    if (s) {
        freeaddrinfo(*result);
        throw std::runtime_error("no se pudo obtener la direccion al conectar");
    }
}

size_t Socket::send(Packet & packet) const {
    while (packet.pendingToSentSize() > 0) {
        ssize_t bytes_sent = ::send(fd, packet.getPendingToSent(),
                                    packet.pendingToSentSize(), MSG_NOSIGNAL);
        if (bytes_sent == 0)
            throw SocketClosed();
        if (bytes_sent == -1)
            throw std::runtime_error("error al enviar datos");
        packet.addSentAmount(bytes_sent);
    }
    return packet.sent();
}

size_t Socket::receive(Packet & packet, size_t size) const{
    std::vector<char> buffer(size);
    while(packet.size() < size) {
        // TODO ver si hacer refactor de esto o dejarlo asi (tema encapsulamiento).
        ssize_t bytes_recv = recv(fd, buffer.data(), buffer.size(), 0);
        if(bytes_recv == 0) {
            std::cout << "aca" << std::endl;
            throw SocketClosed();
        }
        if(bytes_recv == -1)
            throw std::runtime_error("error al recibir datos");
        // TODO hacer que reciba vector?
        packet.addBytes(buffer.data(), bytes_recv);
        buffer.resize(buffer.size() - bytes_recv);
    }
    return packet.size();
}

Socket::Socket(int fd_): fd(fd_) {}
