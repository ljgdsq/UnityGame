cmake_minimum_required (VERSION 3.2)
project (tinyraytracer)

SET(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES *.h *.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} Timer.h)

