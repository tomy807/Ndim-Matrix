
# # 라이브러리 생성
# add_library(matrix STATIC matrix.cpp)
# # 테스트 프로그램 생성
add_executable(block_Matrix block_Matrix.cpp)
add_executable(test_Matrix test_Matrix.cpp)
add_executable(arithmetic_Matrix arithmetic_Matrix.cpp)
add_executable(reduction_Matrix reduction_Matrix.cpp)
add_executable(random_Matrix random_Matrix.cpp)
add_executable(decompse_Matrix decompse_Matrix.cpp)
# 라이브러리와 링크

# gtest 추가
target_link_libraries(block_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)
target_link_libraries(test_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)
target_link_libraries(arithmetic_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)
target_link_libraries(reduction_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)
target_link_libraries(random_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)
target_link_libraries(decompse_Matrix PRIVATE gtest gtest_main Threads::Threads matrix)