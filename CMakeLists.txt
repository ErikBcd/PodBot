cmake_minimum_required(VERSION 3.0.0)
project(Pod VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Pod main.cpp)

add_subdirectory(sleepy-discord)
target_link_libraries(Pod sleepy-discord)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
