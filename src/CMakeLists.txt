add_library(matrix STATIC Matrix.cpp)
target_include_directories(matrix PUBLIC ${CMAKE_SOURCE_DIR}/include)