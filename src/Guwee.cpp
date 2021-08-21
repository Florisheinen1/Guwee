#include <iostream>
#include "Guwee.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"


void printErr(const char* errorMessage) {
	std::cout << "ERROR: " << errorMessage << std::endl;
}
void checkGlErrors() {
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
			case GL_INVALID_ENUM: printErr("INVALID ENUM USED"); break;
			case GL_INVALID_VALUE: printErr("INVALID VALUE USED"); break;
			case GL_INVALID_OPERATION: printErr("INVALID OPERATION USED"); break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: printErr("INVALID FRAMEBUFFER OPERATION USED"); break;
			case GL_OUT_OF_MEMORY: printErr("OUT OF MEMORY"); break;
			default: printErr("UNKNOWN ERROR"); break;
		}
	}
}

void Guwee::initGuwee() {
	std::cout << "Init guwee!" << std::endl;
	checkGlErrors();
	std::cout << "Init vao" << std::endl;

	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialise GLFW" << std::endl;
	} else {
		std::cout << "Successfully initialised GLFW" << std::endl;
	}




	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

	checkGlErrors();
}