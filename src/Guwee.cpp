#include <iostream>
#include "Guwee.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "glad/gl.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glm.hpp"

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

//std::map<char, Character> Characters;

void loadCharacters() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
			continue;
		}

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RED,
					face->glyph->bitmap.width,
					face->glyph->bitmap.rows,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					face->glyph->bitmap.buffer);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(unsigned int) face->glyph->advance.x
		};
		std::cout << ".";
		//Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
	std::cout << std::endl;
	/*
	glGenVertexArrays(1, &FONT_VAO);
	glGenBuffers(1, &FONT_VBO);
	glBindVertexArray(FONT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, FONT_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
}

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
	std::cout << "Init vao" << std::endl;


	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to initialise GLFW" << std::endl;
	} else {
		std::cout << "Successfully initialised GLFW" << std::endl;
	}

	loadCharacters();

	checkGlErrors();


	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);

}