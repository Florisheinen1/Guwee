#include <iostream>
#include <thread>

#include <chrono>


#include "Guwee.hpp"

int main() {
	
	gui::Window window1;
	window1.getWindowCreationHints().title = "Title 1";

	gui::Application app;
	app.addWindow(window1);

	gui::runApplication(app);

	std::cout << std::endl << "===== LEFTOVER CLEANUP =====" << std::endl;

}