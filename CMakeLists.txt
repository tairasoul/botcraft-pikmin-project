cmake_minimum_required(VERSION 3.29)
project(FishingRod-Launch)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libraries)

include_directories(${LIB_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src/include)
include_directories(${CMAKE_SOURCE_DIR}/src/commands/include)
include_directories(/home/$ENV{HOME}/development/c++/Botcraft_Project/Botcraft/botcraft/include)
include_directories(/home/$ENV{HOME}/development/c++/Botcraft_Project/Botcraft/protocolCraft/include)

set(BOTCRAFT_LIB ${LIB_DIR}/libbotcraft.so)
set(PROTOCOLCRAFT_LIB ${LIB_DIR}/libprotocolCraft.so)
set(CMAKE_CXX_STANDARD 20)

find_library(BOTCRAFT_LIB
    NAMES botcraft
    PATHS ${LIB_DIR})

find_library(PROTOCOLCRAFT_LIB
    NAMES protocolCraft
    PATHS ${LIB_DIR})

add_definitions(-DPROTOCOL_VERSION=766)

file(GLOB_RECURSE COMMANDS "src/commands/*.cpp")
add_library(commands SHARED ${COMMANDS})

add_executable(pikmin 
    src/code/GlobalData.cpp
    src/code/IdSplit.cpp
    src/code/Loader.cpp
    src/code/main.cpp
    src/code/PikminClass.cpp)
target_link_libraries(pikmin ${BOTCRAFT_LIB})
target_link_libraries(pikmin ${PROTOCOLCRAFT_LIB})