# add_library(matrix STATIC matrix.cpp)
add_executable(decompse_Matrix decompse_Matrix.cpp)
# 라이브러리와 링크

# gtest 추가
target_link_libraries(decompse_Matrix PRIVATE gtest gtest_main Threads::Threads decompose)