cmake_minimum_required(VERSION 3.0.0)
project(nxml VERSION 0.1.0)

file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE C_SRC_FILES src/*.c)
file(GLOB_RECURSE HEADER_FILES include/*.h)
file(GLOB_RECURSE HEADERPP_FILES include/*.hpp)

add_library(nxml STATIC ${HEADER_FILES} ${HEADERPP_FILES} ${SRC_FILES}  ${C_SRC_FILES})
target_include_directories(nxml PRIVATE include/)
set_property(TARGET nxml PROPERTY CXX_STANDARD 17)