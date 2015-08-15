namespace opengl {

	int init();


	void onResize(int, int);

	void compileShaderProgram();

	void unprojectOnZeroLevel(int, int, float*);


	void setup();

	void render(int, int, unsigned char*);

	void setMatrixes(float, float, float);


	int getWidth();

	int getHeight();

	void setWidth(int);

	void setHeight(int);

}