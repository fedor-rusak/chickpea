#include "GL/glew.h"
#include "GLFW/glfw3.h"

#define GLEW_NO_GLU
#define GLFW_INCLUDE_GL_3

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW test", NULL, NULL);
	printf("111");

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.1f, 0.2f, 0.3f, 0.5f);

	while (1) {
		glfwPollEvents();

		if (glfwWindowShouldClose(window) == 1) break;
		glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(window);
	}
    return 0;
}