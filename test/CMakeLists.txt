
# # 라이브러리 생성
# add_library(matrix STATIC matrix.cpp)
find_package(Threads REQUIRED)
# # 테스트 프로그램 생성
add_executable(matrix_test_2 test_Matrix.cpp)
# 라이브러리와 링크
# target_link_libraries(matrix_test PRIVATE matrix)

# gtest 추가
target_link_libraries(matrix_test_2 PRIVATE gtest gtest_main Threads::Threads)