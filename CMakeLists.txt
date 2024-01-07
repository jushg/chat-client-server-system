cmake_minimum_required(VERSION 3.5)

project(chat_system)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)

file(GLOB SOURCES
    "./include*.hpp"
    "./src/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
target_compile_options(${PROJECT_NAME} PUBLIC -Wall -ggdb)