cmake_minimum_required(VERSION 2.8.12)
project(CheckIntegrationProject)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(inty inty.cpp)
target_link_libraries(inty PUBLIC ${CONAN_LIBS} matplot)
