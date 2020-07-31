#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/exceptions.h"

IllegalArgumentException::IllegalArgumentException(const std::string& message) {
    message_ = message;
}
const char* IllegalArgumentException::what() const throw() {
    return message_.c_str();
}

lastFMError::lastFMError(const std::string& message) {
    message_ = message;
}
const char* lastFMError::what() const throw() {
    return message_.c_str();
}