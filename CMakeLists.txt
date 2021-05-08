#CMakeLists.txt
cmake_minimum_required (VERSION 3.6)
project(PodBot)

add_executable(Pod main.cpp base.cpp)

add_subdirectory(common)

add_subdirectory(deps/sleepy-discord)
target_link_libraries(Pod sleepy-discord Modules)