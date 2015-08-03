#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(1000*x)
#endif

#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "engine/jx_wrapper.hpp"

#include "engine/openal_wrapper.hpp"

#include "engine/freetype_wrapper.hpp"
#include "glfw.cpp"
#include "engine/opengl_wrapper.hpp"

#include "engine.cpp"

int main(int argc, char **args) {
	openal::testAlut("resources/helloworld.wav");

	char* glfwResult = glfw::init(opengl::width, opengl::height, opengl::onResize);
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

	engine::bitmap = freetype::testFTloadCharBitmap('S');

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