#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include <iostream>

#include <vector>

#include "engine/jx_wrapper.hpp"

#include "engine/openal_wrapper.hpp"

#include "engine/freetype_wrapper.hpp"
#include "engine/glfw_wrapper.hpp"


#define GLEW_NO_GLU
#define GLFW_INCLUDE_GL_3

#include <engine/opengl_wrapper.hpp>
#include <GLFW/glfw3.h>

namespace engine {

	namespace bitmap {
		static int width;

		static int height;

		static unsigned char *data;
	}

	void setResolution() {
		int resolution[2];
		glfw::getResolution(resolution);

		opengl::setWidth(resolution[0]);
		opengl::setHeight(resolution[1]);

		opengl::onResize(resolution[0], resolution[1]);
	};


	void getTimeCallback(JXResult *results, int argc) {
		JX_SetDouble(&results[argc], glfw::getTime());
	}

	static std::vector<int> keys = {256, 263, 262, 265, 264};
	static std::string names[] = {"escape", "left", "right", "up", "down"};

	void getInputCallback(JXResult *results, int argc) {
		glfw::pollEvents();

		std::string input = "";

		double xpos, ypos;
		glfw::getCursorPos(&xpos, &ypos);

		float worldPos[2] = {0};
		opengl::unprojectOnZeroLevel((int) xpos, (int) ypos, worldPos);

		char numstr[21];
		sprintf(numstr, "%f", worldPos[0]);
		input += ",\"pointer\": {\"x\":";
		input += numstr;

		sprintf(numstr, "%f", worldPos[1]);
		input += ", \"y\":";
		input += numstr;
		input +="}";


		if (glfw::windowShouldClose() != 0)
			input += ",\"closeWindow\": true";

		for (int i = 0; i < keys.size(); i += 1) {
			if (glfw::getKey(keys[i]) == 1) {
				input += ",\"" + names[i] + "\": true";
			}
		}

		input[0] = ' '; //remove first comma

		input = "{" + input + "}";		

		const char* str = input.c_str();

		JX_SetJSON(&results[argc], str, strlen(str));
	}

	void renderCallback(JXResult *results, int argc) {
		JXValue result = results[0];

		JXValue tempValue;
		JX_GetNamedProperty(&result, "length", &tempValue);
		int length = JX_GetInt32(&tempValue);
		JX_Free(&tempValue);

		std::vector<GLfloat> myData(length);
		for (int i = 0; i < length; i += 1) {
			JX_GetIndexedProperty(&result, i, &tempValue);
			myData[i] = JX_GetDouble(&tempValue);
			JX_Free(&tempValue);
		}

		opengl::setMatrixes(myData[0],myData[1],myData[2]);

		opengl::render(bitmap::width, bitmap::height, bitmap::data);
	}

	void playSound(JXResult *results, int argc) {
		openal::play();
	}


	int init() {
		if (!openal::init()) {
			return 1;
		}

		openal::load("resources/helloworld.wav");


		if(!glfw::init()) {
			return 2;
		}

		setResolution();

		if (!glfw::initWindow(opengl::getWidth(), opengl::getHeight(), opengl::onResize)) {
			return 3;
		}

		if (!opengl::init()) {
			return 4;
		}


		jx::init();
		jx::defineExtension("getTime", getTimeCallback);
		jx::defineExtension("getInput", getInputCallback);
		jx::defineExtension("render", renderCallback);
		jx::defineExtension("playSound", playSound);
		jx::start();


		// int imgWidth, imgHeight, n;
	  	// opengl::textureData = stbi_load("resources/belmont_alpha.png", &imgWidth, &imgHeight, &n, 0);
	 	// free(data);

		freetype::testFTloadCharBitmap('S', &bitmap::width, &bitmap::height, &bitmap::data);

		opengl::compileShaderProgram();

		opengl::setup();

		return 0;
	}

	int terminate() {
		jx::terminate();

		glfw::terminate();

		openal::terminate();

		return 0;
	}


	double getTime() {
		return glfw::getTime();
	}


	void processInput() {
		JXValue tempValue;
		JX_Evaluate(
			"global.processInput();",
			"processInput", &tempValue);
		JX_Free(&tempValue);
	}

	void render() {
		JXValue result;
		JX_Evaluate(
			"process.natives.render(global.arrayData);",
			"render", &result);
		JX_Free(&result);
	}

	bool shouldExit() {
		JXValue tempValue;
		JX_Evaluate(
			"global.exit;",
			"shouldExit", &tempValue);

		bool shouldExit = !JX_IsNullOrUndefined(&tempValue);
		JX_Free(&tempValue);

		return shouldExit;
	}

	void other() {
		jx::loopOnceIOEvents();

		jx::approachGC(25);
	}

	void swapBuffers() {
		glfw::swapBuffers();
	}

}