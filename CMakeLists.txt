cmake_minimum_required(VERSION 3.23)

project(durak)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/headers")

add_executable(durak)

target_sources(durak
PRIVATE
    src/main.cpp
    src/headers/client.hpp
    src/code/client.cpp
)

add_executable(durak-serv)

target_sources(durak-serv
PRIVATE
    src/main-serv.cpp
    src/headers/game.hpp
    src/headers/player.hpp
    src/headers/server.hpp
    src/headers/error.hpp
    src/code/game.cpp
    src/code/player.cpp
    src/code/server.cpp
    src/code/error.cpp
)