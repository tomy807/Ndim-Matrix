cmake_minimum_required(VERSION 3.0)
project(decompose LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(MODULE1_SOURCE_FILES
        src/Decompose.cpp
)

set(CMAKE_MODLUE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_library(decompose 
        ${MODULE1_SOURCE_FILES} 
)

target_include_directories(decompose PUBLIC
    include matrix
)   

target_link_libraries(decompose PUBLIC matrix)
add_subdirectory(test)