#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(1000*x)
#endif


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

	static FT_Bitmap bitmap;

	void setBitmap(FT_Bitmap bitmapValue) {
		bitmap = bitmapValue;
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

		opengl::render(bitmap.width, bitmap.rows, bitmap.buffer);
	}


	int init() {
		openal::testAlut("resources/helloworld.wav");

		glfw::init();

		setResolution();

		char* glfwResult = glfw::initWindow(opengl::getWidth(), opengl::getHeight(), opengl::onResize);
		if (strcmp(glfwResult, "Success!")) {
			puts(glfwResult);
			return 1;
		}

		if (opengl::init() != 0) {
			return 2;
		}


		jx::init();
		jx::defineExtension("getTime", getTimeCallback);
		jx::defineExtension("getInput", getInputCallback);
		jx::defineExtension("render", renderCallback);
		jx::start();


		// int imgWidth, imgHeight, n;
	  	// opengl::textureData = stbi_load("resources/belmont_alpha.png", &imgWidth, &imgHeight, &n, 0);
	 	// free(data);

		setBitmap(freetype::testFTloadCharBitmap('S'));

		opengl::compileShaderProgram();

		opengl::setup();

		return 0;
	}

	int terminate() {
		jx::terminate();

		glfw::terminate();

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

	void sleep(double millis) {
		Sleep(millis);
	}

}