#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#if defined(_MSC_VER)
  #include <windows.h>
#endif

#define GLEW_NO_GLU
#define GLFW_INCLUDE_GL_3

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};

//http://stackoverflow.com/questions/27407774/get-supported-glsl-versions
static const GLchar* vertexShader = "#version 130\n in vec3 vertexPosition_modelspace;\nvoid main(){\ngl_Position.xyz = vertexPosition_modelspace;\ngl_Position.w = 1.0;\n}";
static const GLchar* fragmentShader = "#version 130\nout vec3 color;\nvoid main(){\ncolor = vec3(1,0,0);\n}";


void logFailedGLElement(GLint elementID) {
  GLint maxLength = 10;
  glGetShaderiv(elementID, GL_INFO_LOG_LENGTH, &maxLength);

  printf("%d\n",maxLength);

  // char logbuffer[maxLength];
  // glGetShaderInfoLog(elementID, sizeof(logbuffer), &maxLength, logbuffer);

  //  printf("%s\n", logbuffer);

  glDeleteShader(elementID);
}

void showGlfw3(void) {
  // #if defined(_MSC_VER)
  //   FreeConsole();
  // #endif

  if(!glfwInit()) {
    puts("GLFW3 init SUUUCKS!");
    exit(EXIT_FAILURE);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  //http://stackoverflow.com/questions/10862546/questions-about-glfw-behavior-below-gl-3-2
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW test", NULL, NULL);
  if(window == NULL) {
    puts("Now WindoW!");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    puts("No GLEW!");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  GLuint VertexArrayID;
  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);


  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


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

  GLuint programID = glCreateProgram();
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


  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  do {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(programID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  while(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

  glfwTerminate();
  puts("Nice!");
  exit(EXIT_SUCCESS);
}

#if !defined(mainExists)
  void main(void) {
    showGlfw3();
  }
#endif