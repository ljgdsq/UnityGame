cmake_minimum_required(VERSION 3.24)
project(tinykaboom)


file(GLOB SOURCES *.h *.cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} ${SOURCES} )
