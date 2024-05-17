/******************************************************************************
 * File:        1.2.3.hello_triangle_exercise1.cpp
 * Author:      Davide Tarpini (https://github.com/Sparkhand)
 * Description: This example demonstrates how to draw two triangles next to
 *              each other using glDrawArrays.
 *****************************************************************************/

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <exception>
#include <iostream>
#include <vector>

/* --------- Global vars and constants --------- */

// Window/Screen properties
const struct WINDOW_PROPS
{
    // Screen
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    // Window title
    const char* TITLE = "LearnOpenGL - 1.2.3 - Hello Triangle Exercise 1";
    // Clear color
    const struct CLEAR_COLOR
    {
        float r = 0.2f;
        float g = 0.3f;
        float b = 0.3f;
        float a = 1.0f;
    } CLEAR_COLOR;
} WINDOW_PROPS;

// Shaders (hardcoded)
const char* VERTEX_SHADER_SRC =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* FRAGMENT_SHADER_SRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

/* --------- Additional functions declaration --------- */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* glfwSetup();
std::vector<unsigned int> createShaders();
unsigned int linkShaders(const unsigned int& vertexShader,
                         const unsigned int& fragmentShader);
std::vector<unsigned int> getVObjects();
void clearResources(unsigned int VBO, unsigned int VAO);

/* --------- Main --------- */
int main()
{
    // Initialize GLFW and create window
    GLFWwindow* window;
    try
    {
        window = glfwSetup();
    }
    catch (const std::exception& e)
    {
        glfwTerminate();
        std::cerr << e.what() << std::endl;
        return -1;
    }

    // Load OpenGL function pointers (GLAD init)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shaders
    std::vector<unsigned int> shaders;
    try
    {
        shaders = createShaders();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    if (shaders.size() != 2)
    {
        std::cerr << "ERROR! Shaders vector doesn't have 2 elements" << std::endl;
        return -1;
    }

    unsigned int vertexShader = shaders[0];
    unsigned int fragmentShader = shaders[1];

    // Link shaders
    unsigned int shaderProgram;
    try
    {
        shaderProgram = linkShaders(vertexShader, fragmentShader);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Delete shaders after linking as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data and buffer
    std::vector<unsigned int> vObjects = getVObjects();

    if (vObjects.size() != 2)
    {
        std::cerr << "ERROR! VertexObjects vector doesn't have 2 elements" << std::endl;
        return -1;
    }

    unsigned int VBO = vObjects[0];
    unsigned int VAO = vObjects[1];

    // OPTIONAL: Wireframe mode (unc/comment to toggle)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Render
        glClearColor(WINDOW_PROPS.CLEAR_COLOR.r, WINDOW_PROPS.CLEAR_COLOR.g,
                     WINDOW_PROPS.CLEAR_COLOR.b, WINDOW_PROPS.CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the two triangles
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // (GLFW) Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Once the render loop is finished, terminate GLFW and clear/free resources
    clearResources(VBO, VAO);
    return 0;
}

/* --------- Additional functions definition  --------- */

// Setup and init GLFW
// -----------------------------------------------------------------------------
GLFWwindow* glfwSetup()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

// MacOS compatibility code
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_PROPS.SCR_WIDTH, WINDOW_PROPS.SCR_HEIGHT,
                                          WINDOW_PROPS.TITLE, NULL, NULL);

    if (window == NULL)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

// Create and compile vertex shader and fragment shader
// -----------------------------------------------------------------------------
std::vector<unsigned int> createShaders()
{
    std::vector<unsigned int> shaders;

    // Vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VERTEX_SHADER_SRC, NULL);
    glCompileShader(vertexShader);

    // Check for errors in vertex shader compilation
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaders.push_back(vertexShader);
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SRC, NULL);
    glCompileShader(fragmentShader);

    // Check for errors in fragment shader compilation
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaders.push_back(fragmentShader);
    }

    return shaders;
}

// Link shaders
// -----------------------------------------------------------------------------
unsigned int linkShaders(const unsigned int& vertexShader,
                         const unsigned int& fragmentShader)
{
    // Link vertex and fragment shaders into a shader program
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for errors in shader program linking
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" +
                                 std::string(infoLog));
    }

    return shaderProgram;
}

// Creation of vertex data and buffer objects, and binding
// -----------------------------------------------------------------------------
std::vector<unsigned int> getVObjects()
{
    // Vertices
    const float VERTICES[] = {
        // First triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
        // Second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    }; 

    std::vector<unsigned int> vObjects;

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent
    // strange bugs)
    glBindVertexArray(0);

    vObjects.push_back(VBO);
    vObjects.push_back(VAO);

    return vObjects;
}

// Clear resources at the end of render loop
// -----------------------------------------------------------------------------
void clearResources(unsigned int VBO, unsigned int VAO)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
}

// User input handling
// -----------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    // Close the window on ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Resize the viewport when the window is resized
// -----------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}