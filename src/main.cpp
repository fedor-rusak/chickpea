#include "engine.hpp"

int main(int argc, char **args) {
	if (engine::init() != 0) {return 1;}

	double timeFrame = engine::getTimeFrame(),
		   start = engine::getTime();

	do {
		engine::processInput();		


		if(engine::shouldExit())
			break;


		engine::render();


		engine::other();


		engine::swapBuffers();


		double delay = timeFrame - (engine::getTime() - start)*1000;
		if (delay < 0) delay = 0;
		engine::sleep(delay);
		start = engine::getTime();
	}
	while(true);

	return engine::terminate();
}