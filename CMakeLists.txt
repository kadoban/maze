cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 14)

project (maze)

find_package(Boost 1.58 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(ImageMagick COMPONENTS Magick++)
include_directories(${ImageMagick_INCLUDE_DIRS})

add_definitions(-DMAGICKCORE_QUANTUM_DEPTH=16)
add_definitions(-DMAGICKCORE_HDRI_ENABLE=0)

set(SOURCE_FILES main.cpp maze.cpp image.cpp)

add_executable(maze ${SOURCE_FILES})

target_link_libraries(maze LINK_PUBLIC ${Boost_LIBRARIES})
target_link_libraries(maze LINK_PUBLIC ${ImageMagick_LIBRARIES})
