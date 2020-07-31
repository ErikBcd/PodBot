#pragma once

#include <iostream>
#include <stdio.h>
#include <string.h>

class IllegalArgumentException : public std::exception {
    public:
        explicit IllegalArgumentException(const std::string& message);
        virtual const char* what() const throw();
    private:
        std::string message_;
};

class lastFMError : public std::exception {
    public:
        explicit lastFMError(const std::string& message);
        virtual const char* what() const throw();
    private:
        std::string message_;
};