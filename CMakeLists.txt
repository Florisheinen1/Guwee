cmake_minimum_required(VERSION 3.21.1)
project(Guwee)

set(GUWEE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(GUWEE_INCLUDE_DIR "${GUWEE_DIR}/include")

set(GLAD_DIR "${GUWEE_DIR}/deps/glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")
set(GLAD_SOURCE_DIR "${GLAD_DIR}/src")

# Build GLAD library
add_subdirectory(external/glad/cmake)
glad_add_library(GuweeGlad STATIC LOCATION "${GLAD_DIR}" API "gl:core=3.3")

# Build Guwee library
add_subdirectory(src)

# Build example executables
add_subdirectory(examples)