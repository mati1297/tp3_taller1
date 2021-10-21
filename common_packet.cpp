#include <cstring>
#include <iostream>
#include "common_message.h"

// TODO ver como hacer con el size_ (usar el del vector??)
// TODO constructor con tamanio?

Packet::Packet(): array(0), sent_(0), read_(0), size_(0) {}

void Packet::getBytes(std::string & out, const size_t & size) {
    if((size_ - read_) < size)
        throw std::invalid_argument("se piden mas bytes que los existentes");
    out = std::string(array.begin(), array.begin() + size);
}

void Packet::getBytes(uint16_t & out){
    if((size_ - read_) < 2)
        throw std::invalid_argument("se piden mas bytes que los existentes");
    out = getByte() * 256 + getByte();
}

void Packet::addBytes(char * bytes, const size_t & size) {
    size_t i = 0;
    for(; i < size; i++){
        array.push_back(bytes[i]);
    }
    size_ += i;
}

void Packet::addBytes(const std::string &bytes) {
    size_t i = 0;
    for(; i < bytes.size(); i++){
        array.push_back(bytes[i]);
    }
    size_ += i;
}

void Packet::addBytes(const uint16_t &bytes) {
    char * bytes_ = (char*) &bytes;
    addBytes(bytes_, 2);
}

void Packet::addSentAmmount(const size_t &sent) {
    sent_ += sent;
}

size_t Packet::pendingToSentSize() const {
    return size_ - sent_;
}

size_t Packet::sent() const {
    return sent_;
}

size_t Packet::read() const{
    return read_;
}

void Packet::getPendingToSent(char * buffer, const size_t & size) {
    for(size_t i = 0; (i + sent_) < size_ && i < size; i++){
        buffer[i] = array[i + sent_];
    }
}

void Packet::reset() {
    size_ = 0;
    sent_ = 0;
    read_ = 0;
}

void Packet::resetSent() {
    sent_ = 0;
}

void Packet::resetRead() {
    read_ = 0;
}

size_t Packet::size() {
    return size_;
}

void Packet::print() {
    for(size_t i = 0; i < size_; i++){
        std::cout << array[i];
    }
    std::cout << std::endl;
}

char Packet::getByte() {
    return array[read_++];
}

void Packet::addByte(char byte){
    array.push_back(byte);
    size_++;
}










