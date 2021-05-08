SET(Common_SOURCE
    json.hpp
    kraken_api.hh kraken_api.cpp 
    Exceptions.cpp
)

add_subdirectory(modules)

ADD_LIBRARY(Common STATIC ${Common_SOURCE})