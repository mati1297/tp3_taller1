#ifndef TP3_TALLER1_COMMON_MESSAGE_H
#define TP3_TALLER1_COMMON_MESSAGE_H

#include <string>
#include <vector>

class Packet {
private:
    std::vector<char> array;
    size_t sent_;
    size_t read_;
    size_t size_;

public:
    Packet();

    // TODO copia y eso

    void addBytes(char * bytes, const size_t & size);

    void addBytes(const std::string & bytes);

    void addBytes(const uint16_t & bytes);

    void addSentAmmount(const size_t & sent);

    size_t pendingToSentSize() const;

    size_t sent() const;

    void reset();

    void resetSent();

    size_t size();

    void getPendingToSent(char *buffer, const size_t &size);

    void print();

    void resetRead();

    size_t read() const;

    char getByte();

    void addByte(char byte);

    void getBytes(std::string &out, const size_t &size);

    void getBytes(uint16_t &out);
};


#endif //TP3_TALLER1_COMMON_MESSAGE_H
