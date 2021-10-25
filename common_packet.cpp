#include <cstring>
#include <iostream>
#include <string>
#include "common_packet.h"

// TODO ver como hacer con el size_ (usar el del vector??)
// TODO constructor con tamanio?

Packet::Packet(): array(0), sent_(0), read_(0), size_(0) {}

void Packet::getBytes(std::string & out, const uint32_t & size) {
    if ((size_ - read_) < size)
        throw std::invalid_argument("se piden mas bytes que los existentes");
    out = std::string(array.begin() + read_, array.begin() + read_ + size);
}

void Packet::getBytes(uint16_t & out){
    if ((size_ - read_) < 2)
        throw std::invalid_argument("se piden mas bytes que los existentes");
    char bytes [] = {getByte(), getByte()};
    out = (bytes[1] << 8) + bytes[0];
}

void Packet::addBytes(char * bytes, const uint32_t & size) {
    size_t i = 0;
    for (; i < size; i++)
        array.push_back(bytes[i]);
    size_ += i;
}

void Packet::addBytes(const std::string &bytes) {
    size_t i = 0;
    for (; i < bytes.size(); i++)
        array.push_back(bytes[i]);
    size_ += i;
}

void Packet::addBytes(const uint16_t &bytes) {
    char * bytes_ = (char*) &bytes;
    addBytes(bytes_, 2);
}

void Packet::addSentAmount(const size_t &sent) {
    sent_ += sent;
}

uint32_t Packet::pendingToSentSize() const {
    return size_ - sent_;
}

uint32_t Packet::sent() const {
    return sent_;
}

const char * Packet::getPendingToSent() const {
    return array.data() + sent_;
}

uint32_t Packet::size() {
    return size_;
}

char Packet::getByte() {
    return array[read_++];
}

/*void Packet::addByte(char byte){
    array.push_back(byte);
    size_++;
}*/











