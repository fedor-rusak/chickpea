#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine.hpp"

int main(int argc, char **args) {
	openal::testAlut("resources/helloworld.wav");

	glfw::init();

	engine::setResolution();

	char* glfwResult = glfw::initWindow(opengl::getWidth(), opengl::getHeight(), opengl::onResize);
	if (strcmp(glfwResult, "Success!")) {
		puts(glfwResult);
		return 1;
	}

	jx::init();
	jx::defineExtension("testMethod", engine::glfwGetTime);
	jx::defineExtension("getInput", engine::glfwGetInput);
	jx::defineExtension("render", engine::renderCallback);
	jx::start();


	// int imgWidth, imgHeight, n;
  	// opengl::textureData = stbi_load("resources/belmont_alpha.png", &imgWidth, &imgHeight, &n, 0);
 	// free(data);

	engine::setBitmap(freetype::testFTloadCharBitmap('S'));

	opengl::compileShaderProgram();

	opengl::setup();

	do {
		float start = glfw::getTime();


		engine::processInput();		


		if(engine::shouldExit())
			break;


		engine::render();


		jx::loopOnceIOEvents();


		jx::approachGC(25);


		glfw::swapBuffers();


		// std::cout << glfw::getTime() - start << std::endl;


		Sleep(5);
	}
	while(true);

	jx::terminate();
	puts("JXCore stopped!");

	glfw::terminate();
	puts("GLFW3 terminated!");

	return 0;
}