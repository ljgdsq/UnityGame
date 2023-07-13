cmake_minimum_required(VERSION 3.24)
project(tinyrenderer)



file(GLOB SRC *.cpp *.h)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} ${SRC})
