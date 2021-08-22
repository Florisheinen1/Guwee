#include <string>
#include <vector>
#include <memory>
#include <mutex>

// Define so that glfw and glad can be defined in any order
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace gui {
	class GUI_EXCEPTION : public std::runtime_error {
		public:
			GUI_EXCEPTION() : std::runtime_error(std::string("GUI:") + " An exception occurred") {}
			GUI_EXCEPTION(const std::string &msg) : std::runtime_error(std::string("GUI:") + msg) {}
	};

	class Element {
		public:
			virtual std::unique_ptr<Element> clone() const = 0;

			virtual void onStart() = 0;
			virtual void onDraw() = 0;
			virtual void onStop() = 0;
	};

	struct WindowCreationHints {
		int width = 640;
		int height = 480;
		std::string title = "";
		int resizable = GLFW_TRUE;
		int visible = GLFW_TRUE;
		int decorated = GLFW_TRUE;
		int focused = GLFW_TRUE;
		//GLboolean auto_iconify = GL_TRUE;
		//GLboolean floating = GL_FALSE;
		int maximized = GLFW_FALSE;
		//GLboolean center_cursor = GL_TRUE;
		int transparent_frame_buffer = GLFW_FALSE;
		int refresh_rate = GLFW_DONT_CARE;
	};

	class Window {
		public:
			Window();
			Window(const Window &other);
			~Window();

			WindowCreationHints& getWindowCreationHints();
			void setWindowCreationHints(WindowCreationHints hints);

			void setContext(const Element& element);

			void onOpen();
			void onDraw();
			void onClose();

			void onCloseEvent();
			void onFramebufferResizeEvent(int width, int height);
			void onWindowRefreshEvent();

			bool wantsToOpen() const;
			bool wantsToClose() const;

			void checkForGlErrors() const;
		private:
			bool windowWantsToOpen;
			bool windowWantsToClose;

			GLFWwindow* glfwWindow;
			WindowCreationHints creationHints;

			std::unique_ptr<Element> context;
			bool hasContext;

			void setupCallbacks();
	};

	class Application {
		public:
			Application();
			Application(const Application &other);
			Application &operator =(const gui::Application &);

			bool shouldStop() const;

			void onStart();
			void onTick();
			void onGlfwErrorCallback(int code, const char* description);

			void addWindow(Window window);

		private:
			bool appShouldStop;

			mutable std::mutex windowsMutex;
			std::vector<std::unique_ptr<Window>> windows;
			
			bool updateWindowsOpenState();
			void checkGlErrorsFromWindows();
	};

	class Shader {
		public:
			Shader() = delete;

			static unsigned int compileShaderSource(const char* &vertex, const char* &fragment);

			class SHADER_EXCEPTION : public GUI_EXCEPTION {
				public:
					SHADER_EXCEPTION() : GUI_EXCEPTION(std::string(":SHADER:") + " Encountered exception") {}
					SHADER_EXCEPTION(const std::string &msg) : GUI_EXCEPTION(std::string(":SHADER:") + msg) {}
			};
			class VERTEX_COMPILATION_EXCEPTION : public SHADER_EXCEPTION {
				public:
					VERTEX_COMPILATION_EXCEPTION() : SHADER_EXCEPTION(std::string(":VERTEX:") + " Failed to compile vertex shader") {}
					VERTEX_COMPILATION_EXCEPTION(const std::string &msg) : SHADER_EXCEPTION(std::string(":VERTEX:") + msg) {}
			};
			class FRAGMENT_COMPILATION_EXCEPTION : public SHADER_EXCEPTION {
				public:
					FRAGMENT_COMPILATION_EXCEPTION() : SHADER_EXCEPTION(std::string(":FRAGMENT:") + " Failed to compile vertex shader") {}
					FRAGMENT_COMPILATION_EXCEPTION(const std::string &msg) : SHADER_EXCEPTION(std::string(":FRAGMENT:") + msg) {}
			};
			class LINK_EXCEPTION : public SHADER_EXCEPTION {
				public:
					LINK_EXCEPTION() : SHADER_EXCEPTION(std::string(":LINK:") + " Failed to link shaders into program") {}
					LINK_EXCEPTION(const std::string &msg) : SHADER_EXCEPTION(std::string(":LINK:") + msg) {}
			};
	};

	void runApplication(Application application);

	class GLAD_INITIALISATION_EXCEPTION : public GUI_EXCEPTION {
		public:
			GLAD_INITIALISATION_EXCEPTION() : GUI_EXCEPTION(std::string(":GLAD:") + " Failed to initialize glad") {}
	};

	class GLFW_INITIALISATION_EXCEPTION : public GUI_EXCEPTION {
		public:
			GLFW_INITIALISATION_EXCEPTION() : GUI_EXCEPTION(std::string(":GLFW:") + " Failed to initialize GLFW") {}
	};

}