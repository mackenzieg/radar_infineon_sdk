cmake_minimum_required(VERSION 3.6)
project(radar_sdk)

set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE SOURCES
        src/*.cpp)

include_directories(include externals/radar_sdk/include)

add_executable(radar_sdk ${SOURCES})

target_link_libraries(radar_sdk PRIVATE ${CMAKE_SOURCE_DIR}/externals/radar_sdk/lib/libradar_sdk.a)