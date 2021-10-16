#include "common_socket.h"
#include "common_message.h"
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <exception>
#include <stdexcept>

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
        close(fd);
        fd = INVALID_FILE_DESCRIPTOR;
    }
}

void Socket::connect(const char * host, const char * port) {
    struct addrinfo * result, * ptr;
    try {
        getAddressInfo(&result, host, port);
    }
    catch(std::exception & e) {
        throw;
    }

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

    try {
        getAddressInfo(&result, nullptr, port);
    }
    catch(std::exception & e){
        throw;
    }
    for(ptr = result; ptr; ptr = ptr->ai_next) {
        int skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(skt != -1){
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

    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

    if (listen(fd, pend_conn))
        throw std::runtime_error("fallo al poner el puerto en escucha");
}

Socket Socket::accept() {
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
    if (host == NULL)
        hints.ai_flags = AI_PASSIVE;
    int s = getaddrinfo(host, port, &hints, result);
    if (s) {
        freeaddrinfo(*result);
        throw std::runtime_error("no se pudo obtener la direccion al conectar");
    }
}

size_t Socket::send(Message & msg) const {
    while (msg.pendingToSentSize() > 0) {
        char buffer[RECV_BUFFER_SIZE];
        msg.getPendingToSent(buffer, RECV_BUFFER_SIZE);
        ssize_t bytes_sent = ::send(fd, buffer,
                                    msg.pendingToSentSize(), MSG_NOSIGNAL);
        if (bytes_sent == 0)
            break;
        if (bytes_sent == -1)
            throw std::runtime_error("error al enviar datos");
        msg.addSentAmmount(bytes_sent);
    }
    return msg.sent();
}

size_t Socket::receive(Message & msg, size_t size) const{
    msg.reset();
    while(msg.size() < size) {
        char buffer[RECV_BUFFER_SIZE];
        ssize_t bytes_recv = recv(fd, buffer, RECV_BUFFER_SIZE, 0);
        if(bytes_recv == 0)
            break;
        if(bytes_recv == -1)
            throw std::runtime_error("error al recibir datos");
        msg.addBytes(buffer, bytes_recv);
    }
    return msg.size();
}

Socket::Socket(int fd_): fd(fd_) {}
