#ifndef TP3_TALLER1_MISSING_PARAMETER_H
#define TP3_TALLER1_MISSING_PARAMETER_H

#include <exception>
#include <utility>
#include <string>


class MissingParameter: public std::exception {
    const std::string text;

public:
    explicit MissingParameter(std::string text_): text(std::move(text_)) {}

    const char * what() const noexcept override {
        return text.c_str();
    }
};


#endif //TP3_TALLER1_MISSING_PARAMETER_H
