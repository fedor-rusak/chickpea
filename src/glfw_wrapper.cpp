#include <engine/glfw_wrapper.hpp>

#include <iostream>

namespace glfw {

	static GLFWwindow* window;

	GLFWwindow* getWindow() {
		return window;
	}

	static void (*onResizeCallback)(int, int);

	void onResize(GLFWwindow* window, int iWidth, int iHeight) {
		onResizeCallback(iWidth, iHeight);
	}

	char* init(int width, int height, void (*onResizeFunction)(int, int)) {
		if(!glfwInit())
			return("GLFW3 init SUUUCKS!");

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		//http://stackoverflow.com/questions/10862546/questions-about-glfw-behavior-below-gl-3-2
		// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(width, height, "GLFW test", NULL, NULL);
		if(window == NULL) {
			glfwTerminate();
			return "No WindoW!";
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			return "NO GLEW";
		}

		onResizeCallback = onResizeFunction;
		glfwSetWindowSizeCallback(window, onResize);

		glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

		return "Success!";
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

	std::string getWrappedInput() {
		glfwPollEvents();

		std::string input = "";

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		// std::cout << xpos << " " << ypos << std::endl;


		if (glfwWindowShouldClose(window) != 0)
			input += ",\"closeWindow\": true";

		for (int i = 0; i < keys.size(); i += 1) {
			if (glfwGetKey(window, keys[i]) == GLFW_PRESS) {
				input += ",\"" + names[i] + "\": true";
			}
		}

		input[0] = ' '; //remove first comma

		input = "{" + input + "}";		

		return input;
	}

}