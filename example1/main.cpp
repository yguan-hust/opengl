#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

// comment

GLuint program;
GLint attribute_coord2d;

  const char *vs_source =
  "#version 410\n"
  "in vec3 vp;"
  "void main(void){"
  "gl_Position = vec4(vp, 1.0);"
  "}";

  const char *fs_cource =
  "#version 410\n"
  "out vec4 outputColor;"
  "void main(void){"
  "outputColor = vec4(0.5, 0.0, 0.5, 1.0);"
  "}";

int init_resources()
{
  GLint compile_ok = GL_FALSE, link_ok = GL_FALSE;
  
  GLuint vs = glCreateShader(GL_VERTEX_SHADER);

  glShaderSource(vs, 1, &vs_source, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {

      GLint infoLogLength;
      glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(vs, infoLogLength, NULL, strInfoLog);

      fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
      delete[] strInfoLog;
      fprintf(stderr, "error in vertex error\n");
      return 0;
    }
  
  GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

  glShaderSource(fs, 1, &fs_cource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &compile_ok);
  if (compile_ok == GL_FALSE)
    {
      GLint infoLogLength;
      glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar *strInfoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(fs, infoLogLength, NULL, strInfoLog);

      fprintf(stderr, "Compile failure in shader:\n%s\n", strInfoLog);
      delete[] strInfoLog;

      fprintf(stderr, "error in fragment shader\n");
      return 0;
    }
  
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok)
    {
      fprintf(stderr, "glinkprogram\n");
      return 0;
    }
  
  const char* attribute_name = "vp";
  attribute_coord2d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord2d == -1)
    {
      fprintf(stderr, "cannot bind attribute");
      return 0;
    }
  
  return 1;
}

void onDisplay()
{
  /*
  GLfloat points[] = {
    0.0, 0.8, 1.5,
    0.8, -0.8, 0.0,
    -0.8, -0.8, 0.0,
  };
  */

    GLfloat points[] = {
    -0.8, -0.8, 0.0,
    0.8, -0.8, 0.0,

        0.0, 0.8, 0.0,
  };
  
  GLuint vbo = 0;
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points, GL_STATIC_DRAW);

  GLuint vao = 0;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
}

void free_resources()
{
  glDeleteProgram(program);
}

static void error_callback(int error, const char* description)
{
  fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{

  
  
  GLFWwindow* window;
  glfwSetErrorCallback(error_callback);
  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window)
    {
    glfwTerminate();
    exit(EXIT_FAILURE);
    }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK )
    {
      //      throw std::runtime_error("glew fails to start.");
      fprintf(stderr, "glew error\n");
    }

  // get version info
  const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
  const GLubyte* version = glGetString (GL_VERSION); // version as a string
  printf ("Renderer: %s\n", renderer);
  printf ("OpenGL version supported %s\n", version);

  // tell GL to only draw onto a pixel if the shape is closer to the viewer
  glEnable (GL_DEPTH_TEST); // enable depth-testing
  glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

  glfwSetKeyCallback(window, key_callback);
  
  GLint maxV;
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxV);
  fprintf(stderr, "maxv: %d\n", maxV);
  
  init_resources();
  
  while (!glfwWindowShouldClose(window))
    {
    
    onDisplay();
    glfwSwapBuffers(window);
    glfwPollEvents();
    
    }
  free_resources();
  
  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
