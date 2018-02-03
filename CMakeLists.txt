cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

project (maze)

find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

add_executable(maze main.cpp)

target_link_libraries(maze LINK_PUBLIC ${Boost_LIBRARIES})
