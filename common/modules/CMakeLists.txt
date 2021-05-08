add_library(Modules STATIC 
    Module.hh Module.cpp Module_Kraken.cpp
)

target_link_libraries(Modules sleepy-discord Common)