#ifndef EXCEPTIONS_CPP
#define EXCEPTIONS_CPP
#include <iostream>
#include <string.h>

class IllegalArgumentException : public std::exception {
    public:
        explicit IllegalArgumentException(const std::string& message) {
            message_ = message;
        }
        virtual const char* what() const throw() {
            return message_.c_str();
        }
    private:
        std::string message_;
};

#endif