cmake_minimum_required(VERSION 3.14)
project(AntonymServer C)
set(CMAKE_C_STANDARD 11)

file(GLOB SRC_FILES src/*.c)
file(GLOB INC_FILES include/*.h)
include_directories(include/ enet/include/)
add_executable(AntonymServer main.c ${SRC_FILES} ${INC_FILES})
target_link_libraries(AntonymServer pthread winmm ws2_32)