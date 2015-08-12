#if defined(_MSC_VER)
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(1000*x)
#endif

#include <iostream>

#include "engine/jx_wrapper.hpp"

#include "engine/openal_wrapper.hpp"

#include "engine/freetype_wrapper.hpp"
#include "engine/glfw_wrapper.hpp"
#include "engine/opengl_wrapper.hpp"

namespace engine {

	void glfwGetTime(JXResult*, int);

	void setBitmap(FT_Bitmap);

	void glfwGetInput(JXResult*, int);

	void renderCallback(JXResult*, int);

	void processInput();

	void render();

	bool shouldExit();

}