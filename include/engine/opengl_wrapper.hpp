namespace opengl {

	static int width=600, height=600;

	static GLfloat g_vertex_data[] = {
		-1.0f,-1.0f,0.0,
		-1.0f,1.0f,0.0,
		1.0f,-1.0f,0.0,
		1.0f,1.0f,0.0
	};

	static GLfloat g_texture_coord_data[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	static GLshort indices[] = {0,1,2, 1,2,3};

	//http://stackoverflow.com/questions/27407774/get-supported-glsl-versions
	static const GLchar* vertexShader =
		"#version 130\n"
		"uniform mat4 mProjection, mModelView;\n"
		"in vec3 vertexPosition_modelspace;\n"
		"in vec2 textureUV;\n"
		"out vec2 vTextureCoord;\n"
		"void main(){\n"
		"	vTextureCoord = textureUV;\n"
		"	gl_Position = mProjection * mModelView * vec4(vertexPosition_modelspace,1.0f);\n"
		"}";

	static const GLchar* fragmentShader =
		"#version 130\n"
		"in vec2 vTextureCoord;"
		"out vec4 vFragColor;\n"
		"uniform sampler2D tex;\n"
		"void main(void) {\n"
		"	vFragColor = texture(tex, vTextureCoord);\n"
		"}";

	static GLuint programID;

	void onResize(int, int);

	void compileShaderProgram();

	void unprojectOnZeroLevel(int, int, float*);

	void setup();

	void render(int, int, unsigned char*);

	void setMatrixes(float, float, float);

}