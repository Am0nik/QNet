cmake_minimum_required(VERSION 3.20)

project(QNet LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(QNet
    src/main.cpp
    src/capture/packet_capture.cpp
)

target_include_directories(QNet PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)