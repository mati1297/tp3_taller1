#ifndef TP3_TALLER1_COMMON_PACKET_H
#define TP3_TALLER1_COMMON_PACKET_H

#include <string>
#include <vector>

class Packet {
private:
    std::vector<char> array;
    uint32_t sent_;
    uint32_t read_;
    uint32_t size_;

public:
    Packet();

    // TODO copia y eso

    void addBytes(char * bytes, const uint32_t & size);

    void addBytes(const std::string & bytes);

    void addBytes(const uint16_t & bytes);

    void addSentAmount(const size_t & sent);

    uint32_t pendingToSentSize() const;

    uint32_t sent() const;

    uint32_t size();

    const char * getPendingToSent() const;

    char getByte();

    //void addByte(char byte);

    void getBytes(std::string &out, const uint32_t &size);

    void getBytes(uint16_t &out);
};


#endif //TP3_TALLER1_COMMON_PACKET_H
