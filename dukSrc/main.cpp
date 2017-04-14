#include "engine.hpp"
#include <iostream>

int main(int argc, char **args) {
	if (engine::init() != 0) {return 1;}

	do {
		engine::processInput();		


		if(engine::shouldExit())
			break;


		engine::render();


		engine::other();


		engine::swapBuffers();
	}
	while(true);

	return engine::terminate();
}