namespace glfw {

	void getResolution(int*);

	char* init();

	char* initWindow(int, int, void (*)(int, int));

	void terminate();

	void swapBuffers();

	float getTime();

	void pollEvents();

	void getCursorPos(double*,double*);

	bool windowShouldClose();

	int getKey(int);

}