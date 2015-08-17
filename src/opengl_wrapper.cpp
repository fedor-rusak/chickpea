#include <iostream>

#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <GL/glew.h>

#include <engine/opengl_wrapper.hpp>

namespace opengl {

	static float g_vertex_data[] = {
		-1.0f,-1.0f,0.0,
		-1.0f,1.0f,0.0,
		1.0f,-1.0f,0.0,
		1.0f,1.0f,0.0
	};

	static float g_texture_coord_data[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	static short indices[] = {0,1,2, 1,2,3};

	//http://stackoverflow.com/questions/27407774/get-supported-glsl-versions
	static const char* vertexShader =
		"#version 130\n"
		"uniform mat4 mProjection, mModelView;\n"
		"in vec3 vertexPosition_modelspace;\n"
		"in vec2 textureUV;\n"
		"out vec2 vTextureCoord;\n"
		"void main(){\n"
		"	vTextureCoord = textureUV;\n"
		"	gl_Position = mProjection * mModelView * vec4(vertexPosition_modelspace,1.0f);\n"
		"}";

	static const char* fragmentShader =
		"#version 130\n"
		"in vec2 vTextureCoord;"
		"out vec4 vFragColor;\n"
		"uniform sampler2D tex;\n"
		"void main(void) {\n"
		"	vFragColor = texture(tex, vTextureCoord);\n"
		"}";

	static int programID;

	bool init() {
		glewExperimental = GL_TRUE;
		return (glewInit() == GLEW_OK);
	}


	static int width=300, height=300;

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	void setWidth(int widthValue) {
		width = widthValue;
	}

	void setHeight(int heightValue) {
		height = heightValue;
	}

	static glm::mat4x4 mProjMatrix = glm::perspective(45.0f, width*1.0f/height, 0.1f, 100.0f);

	static glm::mat4x4 mModelViewMatrix = glm::inverse(glm::translate(glm::mat4(), glm::vec3(0,0,6.0)));
	static glm::mat4x4 unprojectModelViewMatrix = glm::inverse(glm::translate(glm::mat4(), glm::vec3(0,0,6.0)));

	static glm::mat4x4 modelMatrix = glm::translate(glm::mat4(), glm::vec3(0,0.0,0));

	static unsigned char *textureData;

	void onResize(int iWidth, int iHeight) {
		if (iWidth != 0 && iHeight != 0) {
			glViewport(0, 0, iWidth, iHeight);
			width=iWidth, height=iHeight;
			mProjMatrix = glm::perspective(45.0f, width*1.0f/height, 0.1f, 100.0f);
		}
	}

	void logFailedGLElement(GLint elementID) {
		GLint maxLength = 10;
		glGetShaderiv(elementID, GL_INFO_LOG_LENGTH, &maxLength);

		char logbuffer[1024];
		glGetShaderInfoLog(elementID, sizeof(logbuffer), &maxLength, logbuffer);

		printf("%s\n", logbuffer);

		glDeleteShader(elementID);
	}

	void compileShaderProgram() {
		GLint glResult = GL_FALSE;

		GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertexShaderID, 1, &vertexShader , NULL);
			glCompileShader(vertexShaderID);
		glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &glResult);

		if (glResult == GL_FALSE) {
			logFailedGLElement(vertexShaderID);
			exit(EXIT_FAILURE);
		}

		GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShaderID, 1, &fragmentShader , NULL);
			glCompileShader(fragmentShaderID);
		glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &glResult);

		if (glResult == GL_FALSE) {
			logFailedGLElement(fragmentShaderID);
			exit(EXIT_FAILURE);
		}

		programID = glCreateProgram();
			glAttachShader(programID, vertexShaderID);
			glAttachShader(programID, fragmentShaderID);
			glLinkProgram(programID);
			glGetProgramiv(programID, GL_LINK_STATUS, &glResult);

		if (glResult == GL_FALSE) {
			logFailedGLElement(programID);
			exit(EXIT_FAILURE);
		}

		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}


	void unproject(int winX, int winY, int winZ, glm::mat4x4 mvMatrix, glm::mat4x4 pMatrix, int vpWidth, int vpHeight, float* result) {
		float x = 2.0 * (winX - 0)/vpWidth - 1,
			  y = 2.0 * (winY - 0)/vpHeight - 1,
			  z = 2.0 * winZ - 1;

		glm::mat4x4 vpMatrix = pMatrix * mvMatrix;

		glm::vec4 n = glm::inverse(vpMatrix) * glm::vec4(x,y,z,1.0);


		result[0] = n[0]/n[3];
		result[1] = n[1]/n[3];
		result[2] = n[2]/n[3];
	}

	void unprojectOnZeroLevel(int x, int y, float* result) {
		float nearArr[3];
		float farArr[3];
		unproject(x,y,0, unprojectModelViewMatrix, mProjMatrix, width, height, nearArr);
		unproject(x,y,1, unprojectModelViewMatrix, mProjMatrix, width, height, farArr);

		float t = -nearArr[2] / (farArr[2] - nearArr[2]);

		// float tempZ = near[2] + (far[2] - near[2]) * t;
		float mapClickX = nearArr[0] + (farArr[0] - nearArr[0]) * t;
		float mapClickY = -(nearArr[1] + (farArr[1] - nearArr[1]) * t);

		result[0] = mapClickX;
		result[1] = mapClickY;
	}

	void setup() {
		glClearColor(0.3f, 0.4f, 0.1f,1.0f);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void render(int imgWidth, int imgHeight, unsigned char* imgBuffer) {
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);


		GLuint vertexbuffer;
		glGenBuffers(1, &vertexbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*myData.size(), myData.data(), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_data), g_vertex_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint textureUVbuffer;
		glGenBuffers(1, &textureUVbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, textureUVbuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_texture_coord_data), g_texture_coord_data, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint indexBufferID;
	    glGenBuffers(1, &indexBufferID);
	    glBindBuffer(GL_ARRAY_BUFFER, indexBufferID);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLuint textureID;
		glGenTextures(1, &textureID);

		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D( GL_TEXTURE_2D, 0,	GL_RGBA,	imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE,	imgBuffer);

		glBindTexture(GL_TEXTURE_2D, 0);


		glUseProgram(programID);


		glUniformMatrix4fv(glGetUniformLocation(programID, "mModelView"), 1, GL_FALSE, glm::value_ptr(mModelViewMatrix * modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(programID, "mProjection"), 1, GL_FALSE, glm::value_ptr(mProjMatrix));


		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glEnableVertexAttribArray(glGetAttribLocation(programID, "vertexPosition_modelspace"));
		glVertexAttribPointer(glGetAttribLocation(programID, "vertexPosition_modelspace"), 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, textureUVbuffer);
		glEnableVertexAttribArray(glGetAttribLocation(programID, "textureUV"));
		glVertexAttribPointer(glGetAttribLocation(programID, "textureUV"), 2, GL_FLOAT, GL_FALSE, 0, 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawElements(GL_TRIANGLES, (sizeof(indices)/sizeof(*indices)), GL_UNSIGNED_SHORT, 0);


		glDeleteBuffers(1, &vertexbuffer);
		glDeleteBuffers(1, &textureUVbuffer);
		glDeleteBuffers(1, &indexBufferID);

		glDeleteTextures(1, &textureID);

		glDeleteVertexArrays(1, &VertexArrayID);
	}

	void setMatrixes(float xVal, float yVal, float zVal) {
		mModelViewMatrix = glm::inverse(glm::translate(glm::mat4(), glm::vec3(xVal,yVal,zVal)));
		unprojectModelViewMatrix = glm::inverse(glm::translate(glm::mat4(), glm::vec3(xVal,-yVal,zVal)));
	}

}