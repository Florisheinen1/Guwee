cmake_minimum_required(VERSION 3.21.1)

add_library(Guwee STATIC "Guwee.cpp")
target_link_libraries(Guwee PUBLIC GuweeGlad GuweeGlfw GuweeFreetype)
target_include_directories(Guwee PUBLIC
	"${GUWEE_INCLUDE_DIR}"
	"${GLAD_INCLUDE_DIR}"
	"${GLFW_INCLUDE_DIR}"
	"${FREETYPE_INCLUDE_DIR}"
	"${GLM_INCLUDE_DIR}")

add_executable(FontTest  fontTest.cpp)
target_link_libraries(FontTest PUBLIC GuweeGlad GuweeGlfw GuweeFreetype)
target_include_directories(FontTest PUBLIC
	"${GUWEE_INCLUDE_DIR}"
	"${GLAD_INCLUDE_DIR}"
	"${GLFW_INCLUDE_DIR}"
	"${FREETYPE_INCLUDE_DIR}"
	"${GLM_INCLUDE_DIR}")