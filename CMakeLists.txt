cmake_minimum_required(VERSION 3.21.1)
project(Guwee)

set(GUWEE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(GUWEE_INCLUDE_DIR "${GUWEE_DIR}/include")

# Build Guwee library
add_subdirectory(src)

# Build example executables
add_subdirectory(examples)