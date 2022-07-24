cmake_minimum_required(VERSION 3.0.0)
project(nxml-demo VERSION 0.1.0)

add_executable(nxml-demo demo.cpp)

set_property(TARGET nxml-demo PROPERTY CXX_STANDARD 17)

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

add_subdirectory(${ROOT_DIR}/nxml ${CMAKE_CURRENT_BINARY_DIR}/nxml)

target_include_directories(nxml-demo PRIVATE ${ROOT_DIR}/nxml/include)
target_link_libraries(nxml-demo harmony-core nxml)