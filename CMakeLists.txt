cmake_minimum_required(VERSION 3.13)
project(HW5)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-m32")

link_directories(Include)

aux_source_directory(src SRC_DIR)

set(SRCS ${SRC_DIR})

add_executable(HW5 main.cpp ${SRCS})

target_link_libraries(HW5 -lOpenGL32 -lfreeglut -lglu32)