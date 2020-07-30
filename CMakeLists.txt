cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 14)
project(sa_opendrive)

set(OpenCV_DIR C:/OpenCV/)
set(OpenCV_FOUND 1)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(sa_opendrive main.cpp)
target_link_libraries(sa_opendrive ${OpenCV_LIBS})