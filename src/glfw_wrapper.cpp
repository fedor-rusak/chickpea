#define GLEW_NO_GLU
#define GLFW_INCLUDE_GL_3

#include <GLFW/glfw3.h>


#include <vector>

#include <iostream>

namespace glfw {

	static GLFWwindow* window;

	static void (*onResizeCallback)(int, int);

	void onResize(GLFWwindow* window, int iWidth, int iHeight) {
		onResizeCallback(iWidth, iHeight);
	}

	void getResolution(int *resolution) {

		const GLFWvidmode* vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		resolution[0] = vidMode->width;
		resolution[1] = vidMode->height;
	}

	bool init() {
		return glfwInit();
	}

	bool initWindow(int width, int height, void (*onResizeFunction)(int, int)) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//http://stackoverflow.com/questions/10862546/questions-about-glfw-behavior-below-gl-3-2
		// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, "GLFW test", NULL, NULL);
		if(window == NULL) {
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(window);


		onResizeCallback = onResizeFunction;
		glfwSetWindowSizeCallback(window, onResize);

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
		glfwSwapInterval(1);

		return true;
	}

	void swapBuffers() {
		return glfwSwapBuffers(window);
	}

	void terminate() {
		glfwTerminate();
	}

	float getTime() {
		return glfwGetTime();
	}

	static std::vector<int> keys = {GLFW_KEY_ESCAPE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN};
	static std::string names[] = {"escape", "left", "right", "up", "down"};

	void pollEvents() {
		glfwPollEvents();
	}

	void getCursorPos(double *xpos,double *ypos) {
		glfwGetCursorPos(window, xpos, ypos);
	}

	bool windowShouldClose() {
		return glfwWindowShouldClose(window);
	}

	int getKey(int keyCode) {
		if (glfwGetKey(window, keyCode) == GLFW_PRESS) {
			return 1;
		}
		else {
			return 0;
		}
	}

}