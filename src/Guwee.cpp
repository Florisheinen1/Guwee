#include <iostream>
#include <thread>
#include <map>

#include "Guwee.hpp"

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "freetype2/ft2build.h"
#include FT_FREETYPE_H

const float squareVertices[] = {
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.5f, 0.5f,
	-0.5f, 0.5f
};

const char *vertexShaderSourceGui = "#version 330 core\n"
"layout (location = 0) in vec2 position;\n"
"out vec4 vertexColor;\n"
"uniform vec4 uniColor;\n"
//"layout (location = 1) in vec2 textureCoordIn;\n"
//"out vec2 textureCoord;\n"
//"uniform mat4 transformation;\n"
"void main() {\n"
//"  textureCoord = textureCoordIn;\n"
"  vertexColor = uniColor;\n"
"  gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
"}\0";
const char *fragmentShaderSourceGui = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
//"in vec2 textureCoord;\n"
"void main() {\n"
"  FragColor = vertexColor;\n"
"}\0";

const char *vertexShaderSourceFont = "#version 330 core\n"
"layout (location = 0) in vec4 vertex;\n"
"out vec2 TexCoords;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"  gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);\n"
"  TexCoords = vertex.zw;\n"
"}\0";
const char *fragmentShaderSourceFont = "#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"uniform sampler2D text;\n"
"uniform vec3 textColor;\n"
"void main() {\n"
"  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"  color = vec4(textColor, 1.0) * sampled;\n"
"}\0";

/*
void loadCharacters() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return;
	} else {
		std::cout << "Initialised freetype" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "../resources/fonts/arial.ttf", 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	} else {
		std::cout << "Loaded font" << std::endl;
	}
 
	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		std::cout << "Loading character: " << c << std::endl;
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

		std::cout << "Created character struct: " << character.TextureID << ", " << "(" << character.Size.x << character.Size.y << "), (" << character.Bearing.x << character.Bearing.y << "), " << character.Advance << std::endl;

		Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &FONT_VAO);
	glGenBuffers(1, &FONT_VBO);
	glBindVertexArray(FONT_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, FONT_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void drawCharacter(std::string text, float x, float y, float z, float scale, glm::vec3 color) {
	
	std::cout << "Drawing shit" << std::endl;
	std::cout << "--test" << std::endl;
	glUseProgram(SHADER_FONT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

	std::cout << "Shader font: " << SHADER_FONT << std::endl;
	glUseProgram(SHADER_FONT);

	glUniform3f(glGetUniformLocation(SHADER_FONT, "textColor"), 1.0f, 1.0f, 0.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(FONT_VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];
		float xpos = x + ch.Bearing.x * scale;
		float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{xpos,		ypos + h, 	0.0f, 0.0f},
			{xpos,		ypos, 		0.0f, 1.0f},
			{xpos + w,	ypos, 		1.0f, 1.0f},

			{xpos,		ypos + h, 	0.0f, 0.0f},
			{xpos + w,	ypos, 		1.0f, 1.0f},
			{xpos + w,	ypos + h,	1.0f, 0.0f}
		};

		std::cout << "Binging texture: " << ch.TextureID << std::endl;
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);

		glBindBuffer(GL_ARRAY_BUFFER, FONT_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	checkGlErrors();
}
*/

unsigned int SHADER_FONT;
unsigned int SHADER;
unsigned int SQUARE_VAO;
unsigned int FONT_VAO;
unsigned int FONT_VBO;

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	FT_Pos Advance;
};

std::map<char, Character> Characters;

Character xChar;
GLuint xCharVAO;

void loadChars() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Failed to initialise freetype" << std::endl;
		return;
	} else {
		std::cout << "Initialised freetype" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, "../resources/fonts/arial.ttf", 0, &face)) {
		std::cout << "Failed to load font" << std::endl;
		return;
	} else {
		std::cout << "Loaded font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		std::cout << "Failed to load glyph" << std::endl;
		return;
	} else {
		std::cout << "Loaded glyph" << std::endl;
	}

	// Generate texture
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	xChar = {
		texture,
		glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		face->glyph->advance.x
	};

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	std::cout << "Loaded character with texture: " << xChar.TextureID << std::endl;

	float xCharVertices[] = {
		-0.5f, -0.5f,		0.0f, 0.0f,
		 0.5f, -0.5f,		1.0f, 0.0f,
		 0.5f,  0.5f,		1.0f, 1.0f,
		-0.5f,  0.5f,		0.0f, 1.0f
	};

	glGenVertexArrays(1, &xCharVAO);
	glBindVertexArray(xCharVAO);

	GLuint xCharVBO;
	glGenBuffers(1, &xCharVBO);
	glBindBuffer(GL_ARRAY_BUFFER, xCharVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(xCharVertices), xCharVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindVertexArray(0);
}

void drawCharX() {
	std::cout << "-- Drawing char" << std::endl;
	glUseProgram(SHADER_FONT);
	
	glUniform3f(glGetUniformLocation(SHADER_FONT, "textColor"), 1.0f, 0.2f, 0.2f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, xChar.TextureID);
	glBindVertexArray(xCharVAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

// Prints the given error to the console.
void printErr(const char* errorMessage) {
	std::cout << "ERROR: " << errorMessage << std::endl;
}
// Checks if there are any GL errors made, and outputs them to console.
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

void testShaderCompilationSuccess(unsigned int shader, bool isVertex) {
	int shaderSuccess;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderSuccess);
	if (!shaderSuccess) {
		glGetShaderInfoLog(shader, 521, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << (isVertex ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

unsigned int gui::Shader::compileShaderSource(const char* &vertex, const char* &fragment) {

	char infoLog[512];
	int createSuccess;
	
	// Compile vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &createSuccess);
	if (!createSuccess) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		throw gui::Shader::VERTEX_COMPILATION_EXCEPTION(infoLog);
	}

	// Compile fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &createSuccess);
	if (!createSuccess) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		throw gui::Shader::FRAGMENT_COMPILATION_EXCEPTION(infoLog);
	}

	// Link shaders into program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &createSuccess);
	if (!createSuccess) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		throw gui::Shader::LINK_EXCEPTION(infoLog);
	}

	// Clear resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

unsigned int createSquareVAO() {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return VAO;
}

void drawGuiElement() {
	glUseProgram(SHADER);

	float timeValue = (float) glfwGetTime();
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
	int vertexColorLocation = glGetUniformLocation(SHADER, "uniColor");
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

	glBindVertexArray(SQUARE_VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

///////////////////////////////////////////////////////////////////

std::mutex runninApplicationMutex; // Guards app
gui::Application app;

std::mutex isGlfwInitialisedMutex; // Guards glfwIsInitialised
bool isGlfwInitialised;

// Will initialise glfw. Throws exception if failed
void initialiseGlfw() {
	const std::scoped_lock lock(isGlfwInitialisedMutex);
	if (glfwInit() == GLFW_FALSE)
		throw gui::GLFW_INITIALISATION_EXCEPTION();
	isGlfwInitialised = true;
}
// Will always terminate glfw.
void terminateGlfw() {
	const std::scoped_lock lock(isGlfwInitialisedMutex);
	glfwTerminate();
	isGlfwInitialised = false;
}
// Will post empty glfw event only if glfw is initialised
void postEmptyGlfwEvent() {
	const std::scoped_lock lock(isGlfwInitialisedMutex);
	if (isGlfwInitialised)
		glfwPostEmptyEvent();
}
// Function to forwards glfw error to current running application.
void glfwErrorCallback(int code, const char * description) {
	app.onGlfwErrorCallback(code, description);
}

// Starts the given application. Waits untill no other application runs.
void gui::runApplication(Application application) {
	const std::scoped_lock runningAppLock(runninApplicationMutex);
	app = application;
	glfwSetErrorCallback(glfwErrorCallback);

	initialiseGlfw();

	app.onStart();

	while (!app.shouldStop()) {
		app.onTick();
		glfwWaitEvents();
	}

	terminateGlfw();
}

// ===== APPLICATION ===== //
gui::Application::Application() {
	this->appShouldStop = false;
}
gui::Application::Application(const gui::Application& other) {
	this->appShouldStop = other.appShouldStop;
	{
		const std::scoped_lock lock(this->windowsMutex, other.windowsMutex);
		for (auto& window : other.windows) {
			this->windows.push_back(std::make_unique<Window>(*window.get()));
		}
	}
}
gui::Application& gui::Application::operator=(const gui::Application& other) {
	if (this == &other) return *this;

	this->appShouldStop = other.appShouldStop;

	{
		const std::scoped_lock lock(this->windowsMutex, other.windowsMutex);
		for (auto& window : other.windows) {
			this->windows.push_back(std::make_unique<Window>(*window.get()));
		}
	}
	return *this;
}

bool gui::Application::shouldStop() const {
	return this->appShouldStop;
}
void gui::Application::onStart() {}
void gui::Application::onTick() {
	bool noWindowsLeft = this->updateWindowsOpenState();

	if (noWindowsLeft)
		this->appShouldStop = true;
	else
		this->checkGlErrorsFromWindows();
}
void gui::Application::onGlfwErrorCallback(int code, const char * description) {
	std::cout << "An error occured with code: " << code << std::endl;
	std::cout << " - Meaning: " << description << std::endl;
}
void gui::Application::addWindow(gui::Window window) {
	const std::scoped_lock lock(this->windowsMutex);
	this->windows.push_back(std::make_unique<Window>(window));
	postEmptyGlfwEvent();
}
// Opens and closes all necessary windows. Returns true if no windows are left.
bool gui::Application::updateWindowsOpenState() {
	const std::scoped_lock lock(this->windowsMutex);
	auto it = this->windows.begin();

	while (it != this->windows.end()) {
		if ((*it)->wantsToOpen())
			(*it)->onOpen();

		if ((*it)->wantsToClose()) {
			(*it)->onClose();
			it = this->windows.erase(it);
		} else {
			++it;
		}
	}

	return this->windows.empty();
}
void gui::Application::checkGlErrorsFromWindows() {
	const std::scoped_lock lock(this->windowsMutex);
	for (auto& window : this->windows) {
		window->checkForGlErrors();
	}
}

// ===== WINDOW ===== //
gui::Window::Window() {
	this->glfwWindow = nullptr;
	this->hasContext = false;
	this->windowWantsToOpen = true;
	this->windowWantsToClose = false;
}
gui::Window::Window(const gui::Window& other) {
	this->glfwWindow = other.glfwWindow;
	this->hasContext = other.hasContext;
	this->creationHints = other.creationHints;

	this->windowWantsToOpen = other.windowWantsToOpen;
	this->windowWantsToClose = other.windowWantsToClose;
	
	if (this->hasContext)
		this->context = other.context.get()->clone();
}
gui::Window::~Window() {
	std::cout << "Deleted window" << std::endl;
}
gui::WindowCreationHints& gui::Window::getWindowCreationHints() {
	return this->creationHints;
}
void gui::Window::setWindowCreationHints(gui::WindowCreationHints hints) {
	this->creationHints = hints;
}
// Copies the given element and sets it as the context of the window.
void gui::Window::setContext(const gui::Element& element) {
	this->context = element.clone();
	this->hasContext = true;
}
// Opens the window. Should only be called from Application.
void gui::Window::onOpen() {
	// Hints that apply to every window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Window specific hints
	glfwWindowHint(GLFW_RESIZABLE, this->creationHints.resizable);
	glfwWindowHint(GLFW_VISIBLE, this->creationHints.visible);
	glfwWindowHint(GLFW_DECORATED, this->creationHints.decorated);
	glfwWindowHint(GLFW_FOCUSED, this->creationHints.focused);
	glfwWindowHint(GLFW_MAXIMIZED, this->creationHints.maximized);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, this->creationHints.transparent_frame_buffer);
	glfwWindowHint(GLFW_REFRESH_RATE, this->creationHints.refresh_rate);

	this->glfwWindow = glfwCreateWindow(this->creationHints.width, this->creationHints.height, this->creationHints.title.c_str(), NULL, NULL);

	// Test if glfw window context was created successfully.
	if (this->glfwWindow == NULL)
		return;

	// Initialise GLAD for this context
	glfwMakeContextCurrent(this->glfwWindow);
	//gladLoaderLoadGL((gladLoadGL)glfwGetProcAddress);

	
	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
		std::cout << "Failed to initialze GLAD" << std::endl;
		return;
	} else {
		std::cout << "Successfully initialised GLAD" << std::endl;
	}

	glViewport(0, 0, this->creationHints.width, this->creationHints.height);
	this->setupCallbacks();
	this->windowWantsToOpen = false;

	SHADER_FONT = gui::Shader::compileShaderSource(vertexShaderSourceFont, fragmentShaderSourceFont);
	SHADER = gui::Shader::compileShaderSource(vertexShaderSourceGui, fragmentShaderSourceGui);
	SQUARE_VAO = createSquareVAO();

	loadChars();
	//loadCharacters();

	if (this->hasContext) {
		this->context->onStart();
	}
	std::cout << "Opened window '" << this->creationHints.title << "'" << std::endl;
}
void gui::Window::onDraw() {
	glfwMakeContextCurrent(this->glfwWindow);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (this->hasContext) {
		//this->context->onDraw();
	}
	drawGuiElement();
	//drawCharacter("Hello World!", 0.1f, 0.1f, 0.1f, 1.0f, glm::vec3(1.0, 0.0, 0.0));
	
	drawCharX();
	glfwSwapBuffers(this->glfwWindow);
}
void gui::Window::onClose() {
	glfwDestroyWindow(this->glfwWindow);
	this->windowWantsToClose = false;
}
void gui::Window::onCloseEvent() {
	this->windowWantsToClose = true;
}
void gui::Window::onFramebufferResizeEvent(int width, int height) {
	glViewport(0, 0, width, height);
}
void gui::Window::onWindowRefreshEvent() {
	this->onDraw();
}
bool gui::Window::wantsToOpen() const {
	return this->windowWantsToOpen;
}
bool gui::Window::wantsToClose() const {
	return this->windowWantsToClose;
}
void gui::Window::checkForGlErrors() const {
	checkGlErrors();
}
void gui::Window::setupCallbacks() {
	// Set user pointer to this class, so callbacks can be reffered to member methods
	glfwSetWindowUserPointer(this->glfwWindow, this);

	// Now set up all callbacks
	auto windowCloseCallback = [] (GLFWwindow* window) {
		static_cast<Window*>(glfwGetWindowUserPointer(window))->onCloseEvent();
	};
	glfwSetWindowCloseCallback(this->glfwWindow, windowCloseCallback);

	auto framebufferSizeCallback = [] (GLFWwindow* window, int width, int height) {
		static_cast<Window*>(glfwGetWindowUserPointer(window))->onFramebufferResizeEvent(width, height);
	};
	glfwSetFramebufferSizeCallback(this->glfwWindow, framebufferSizeCallback);

	auto windowRefreshCallback = [] (GLFWwindow* window) {
		static_cast<Window*>(glfwGetWindowUserPointer(window))->onWindowRefreshEvent();
	};
	glfwSetWindowRefreshCallback(this->glfwWindow, windowRefreshCallback);
}

// ===== ELEMENT ===== //
void gui::Element::onDraw() {
	std::cout << "Default element was drawn" << std::endl;
}