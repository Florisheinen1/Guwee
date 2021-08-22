cmake_minimum_required(VERSION 3.21.1)

add_executable(Example1 example1.cpp)
target_link_libraries(Example1 PUBLIC Guwee)
target_include_directories(Example1 PUBLIC "${GUWEE_INCLUDE_DIR}")