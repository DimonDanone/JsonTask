cmake_minimum_required(VERSION 3.10)
project(TestTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(SOURCE_FILES main.cpp JsonReader.h JsonReader.cpp TLVencoding.h TVLencoding.cpp)
add_executable(TestTask ${SOURCE_FILES})
