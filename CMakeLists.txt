cmake_minimum_required(VERSION 3.21.1)
project(Guwee)

set(GUWEE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(GUWEE_INCLUDE_DIR "${GUWEE_DIR}/include")
set(GUWEE_DEPENDENCY_DIR "${GUWEE_DIR}/deps")

set(GLAD_DIR "${GUWEE_DEPENDENCY_DIR}/glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")
set(GLAD_SOURCE_DIR "${GLAD_DIR}/src")

set(GLFW_INCLUDE_DIR "${GUWEE_DIR}/external/glfw/include")

set(FREETYPE_INCLUDE_DIR "${GUWEE_DIR}/external/freetype/include")

# Build GLAD library
add_subdirectory(external/glad/cmake)
glad_add_library(GuweeGlad STATIC LOCATION "${GLAD_DIR}" API "gl:core=3.3")

# Build GLFW library
add_subdirectory(external/glfw)
add_library(GuweeGlfw ALIAS glfw)

# Build freetype library
add_subdirectory(external/freetype)
add_library(GuweeFreetype ALIAS freetype)

# Build Guwee library
add_subdirectory(src)

# Build example executables
add_subdirectory(examples)