#include "engine.hpp"

namespace engine {

	void glfwGetTime(JXResult *results, int argc) {
		JX_SetDouble(&results[argc], glfw::getTime());
	}

	static FT_Bitmap bitmap;

	void setBitmap(FT_Bitmap bitmapValue) {
		bitmap = bitmapValue;
	}


	static std::vector<int> keys = {GLFW_KEY_ESCAPE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN};
	static std::string names[] = {"escape", "left", "right", "up", "down"};

	void glfwGetInput(JXResult *results, int argc) {
		GLFWwindow* window = glfw::getWindow();

		glfw::pollEvents();

		std::string input = "";

		double xpos, ypos;
		glfw::getCursorPos(&xpos, &ypos);

		float worldPos[2] = {0};
		opengl::unprojectOnZeroLevel((int) xpos, (int) ypos, worldPos);
		std::cout << worldPos[0] << " " << worldPos[1] << std::endl;


		if (glfwWindowShouldClose(window) != 0)
			input += ",\"closeWindow\": true";

		for (int i = 0; i < keys.size(); i += 1) {
			if (glfwGetKey(window, keys[i]) == GLFW_PRESS) {
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

	void processInput() {
		JXValue tempValue;
		JX_Evaluate(
			"global.processInput();",
			"myscript2", &tempValue);
		JX_Free(&tempValue);
	}

	void render() {
		JXValue result;
		JX_Evaluate(
			"process.natives.render(global.arrayData);",
			"myscript2", &result);
		JX_Free(&result);
	}

	bool shouldExit() {
		JXValue tempValue;
		JX_Evaluate(
			"global.exit;",
			"myscript2", &tempValue);

		bool shouldExit = !JX_IsNullOrUndefined(&tempValue);
		JX_Free(&tempValue);

		return shouldExit;
	}
}