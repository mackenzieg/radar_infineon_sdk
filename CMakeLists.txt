cmake_minimum_required(VERSION 3.6)
project(radar_sdk)

SET(Boost_USE_STATIC_LIBS OFF)
SET(Boost_USE_MULTITHREAD ON)
FIND_PACKAGE(Boost 1.67.0 REQUIRED COMPONENTS system program_options)
IF(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ENDIF(Boost_FOUND)

SET(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

set(CMAKE_CXX_STANDARD 11)
file(GLOB_RECURSE SOURCES
        src/*.cpp)

include_directories(include externals/radar_sdk/include /usr/local/include)

add_executable(radar_sdk ${SOURCES})

target_link_libraries(radar_sdk PRIVATE ${CMAKE_SOURCE_DIR}/externals/radar_sdk/lib/libradar_sdk.a /usr/local/lib/libfftw3.a ${USED_LIBS})