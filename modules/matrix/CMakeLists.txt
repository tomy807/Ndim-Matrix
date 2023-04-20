cmake_minimum_required(VERSION 3.0)
project(matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

set(MODULE1_SOURCE_FILES
        src/Matrix.cpp
)

set(CMAKE_MODLUE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_library(matrix 
        ${MODULE1_SOURCE_FILES}
)

target_include_directories(matrix PUBLIC
    include
)   

target_link_libraries(matrix PUBLIC)
add_subdirectory(test)