#define GLEW_NO_GLU
#define GLFW_INCLUDE_GL_3

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace glfw {

		GLFWwindow* getWindow();

		char* init(int, int, void (*)(int, int));

		void terminate();

		void swapBuffers();

		float getTime();

		void pollEvents();

		void getCursorPos(double*,double*);

}