cmake_minimum_required(VERSION 3.21.1)

add_library(Guwee STATIC "Guwee.cpp" "${GUWEE_INCLUDE_DIR}/Guwee.hpp")
target_link_libraries(Guwee PUBLIC GuweeGlad GuweeGlfw)
target_include_directories(Guwee PUBLIC "${GUWEE_INCLUDE_DIR}" "${GLAD_INCLUDE_DIR}" "${GLFW_INCLUDE_DIR}")