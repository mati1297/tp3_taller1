#ifndef TP3_TALLER1_SOCKET_CLOSED_H
#define TP3_TALLER1_SOCKET_CLOSED_H
#include <exception>
#include <utility>


class MissingParameter: public std::exception {
    const std::string text;

public:
    explicit MissingParameter(std::string text_): text(std::move(text_)) {}

    const char * what () const noexcept override {
        return text.c_str();
    }
};


#endif //TP3_TALLER1_SOCKET_CLOSED_H
