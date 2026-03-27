cmake_minimum_required(VERSION 3.10)
project(ParkingLotManagement)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(parking_lot ${SOURCES})

find_path(READLINE_INCLUDE_DIR readline/readline.h
    HINTS /usr/include /usr/local/include /opt/homebrew/include
)
find_library(READLINE_LIBRARY readline
    HINTS /usr/lib /usr/local/lib /opt/homebrew/lib
)

if(READLINE_LIBRARY AND READLINE_INCLUDE_DIR)
    message(STATUS "Found Readline: ${READLINE_LIBRARY}")
    target_include_directories(parking_lot PRIVATE ${READLINE_INCLUDE_DIR})
    target_link_libraries(parking_lot ${READLINE_LIBRARY})
    target_compile_definitions(parking_lot PRIVATE USE_READLINE)
else()
    message(STATUS "Readline not found, using standard input")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(parking_lot PRIVATE -g -Wall)
else()
    target_compile_options(parking_lot PRIVATE -O2)
endif()
