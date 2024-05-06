#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <iostream>

/* Global vars and constants */

// Window-related
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *WINDOW_TITLE = "LearnOpenGL - 1.2.2 - Hello Triangle Indexed";
const struct CLEAR_COLOR {
  float r = 0.2f;
  float g = 0.3f;
  float b = 0.3f;
  float a = 1.0f;
} clear_color;

// Shaders
const char *VERTEX_SHADER_SRC =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *FRAGMENT_SHADER_SRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

/* Additional functions declaration */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

/* Main */
int main() {
  // Initialize GLFW
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // MacOS compatibility code
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // -- GLFW window creation
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // Load OpenGL funciton pointers (GLAD init)
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // -- Shader building and compilation
  // Vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &VERTEX_SHADER_SRC, NULL);
  glCompileShader(vertexShader);

  // Check for errors in vertex shader compilation
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // Fragment shader
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SRC, NULL);
  glCompileShader(fragmentShader);

  // Check for errors in fragment shader compilation
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << infoLog << std::endl;
  }

  // -- Shader linking
  // Link vertex and fragment shaders into a shader program
  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  // Check for errors in shader program linking
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
              << infoLog << std::endl;
  }

  // Delete shaders after linking as they are no longer needed
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // -- Vertex data and buffer
  const float VERTICES[] = {
      // Our triangle
      -0.5f, -0.5f, 0.0f, // left
      0.5f,  -0.5f, 0.0f, // right
      0.0f,  0.5f,  0.0f, // top
  };

  // Vertex buffer object (VBO) and vertex array object (VAO) creation
  unsigned int VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  // Bind VAO first, then bind and set vertex buffer(s), and then configure
  // vertex attributes(s)
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

  // Position attribute (location = 0 in vertex shader) (3 floats per vertex)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
  // strange bugs)
  glBindVertexArray(0);

  // -- OPTIONAL: Wireframe mode (unc/comment to toggle)
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // -- Render loop
  while (!glfwWindowShouldClose(window)) {
    // Input handling
    processInput(window);

    // Render
    glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the two triangles
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Start drawing from index 0 and size 6

    // (GLFW) Swap buffers and poll IO events
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Once the render loop is finished, terminate GLFW, clearing any resources
  // allocated by GLFW
  glfwTerminate();
  return 0;
}

/* Additional functions definition */

// User input handling
// -----------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  // Close the window on ESC
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// Resize the viewport when the window is resized
// -----------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}