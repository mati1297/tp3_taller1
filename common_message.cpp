#include <cstring>
#include <iostream>
#include "common_message.h"

Message::Message(): array(0), sent_(0), read_(0), size_(0) {}

void Message::getBytes(std::vector<char> & out, const size_t & size) {
    if((size_ - read_) < size)
        throw std::invalid_argument("se piden mas bytes que los existentes");
    out = std::vector<char>(array.begin(), array.begin() + size);
}

void Message::getBytes(uint16_t & out){
    if((size_ - read_) < 2)
        throw std::invalid_argument("se piden mas bytes que los existentes");

}

void Message::addBytes(char * bytes, const size_t & size) {
    size_t i = 0;
    for(; i < size; i++){
        array.push_back(bytes[i]);
    }
    size_ += i;
}

void Message::addBytes(const std::string &bytes) {
    size_t i = 0;
    for(; i < bytes.size(); i++){
        array.push_back(bytes[i]);
    }
    size_ += i;
}

void Message::addBytes(const uint16_t &bytes) {
    array.push_back((char) (bytes));
    array.push_back((char) (bytes >> 8));
    size_ += 2;
}

void Message::addSentAmmount(const size_t &sent) {
    sent_ += sent;
}

size_t Message::pendingToSentSize() const {
    return size_ - sent_;
}

size_t Message::sent() const {
    return sent_;
}

size_t Message::read() const{
    return read_;
}

void Message::getPendingToSent(char * buffer, const size_t & size) {
    for(size_t i = 0; (i + sent_) < size_ && i < size; i++){
        buffer[i] = array[i + sent_];
    }
}

void Message::reset() {
    size_ = 0;
    sent_ = 0;
    read_ = 0;
}

void Message::resetSent() {
    sent_ = 0;
}

void Message::resetRead() {
    read_ = 0;
}

size_t Message::size() {
    return size_;
}

void Message::print() {
    for(size_t i = 0; i < size_; i++){
        std::cout << array[i];
    }
    std::cout << std::endl;
}

char Message::getByte() {
    return array[read_++];
}

void Message::addByte(char byte){
    array.push_back(byte);
    size_++;
}










