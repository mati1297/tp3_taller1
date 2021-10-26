#ifndef TP3_TALLER1_COMMON_INVALID_PARAMETER_ADDR_H
#define TP3_TALLER1_COMMON_INVALID_PARAMETER_ADDR_H

#include <exception>
#include <utility>
#include <string>

class InvalidParameterAddr: public std::exception {
    const std::string text;

public:
    explicit InvalidParameterAddr(std::string text_): text(std::move(text_)) {}

    InvalidParameterAddr() = default;

    const char * what() const noexcept override {
        return text.c_str();
    }
};


#endif //TP3_TALLER1_COMMON_INVALID_PARAMETER_ADDR_H
