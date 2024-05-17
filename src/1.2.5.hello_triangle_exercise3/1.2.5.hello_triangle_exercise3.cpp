/******************************************************************************
 * File:        1.2.5.hello_triangle_exercise3.cpp
 * Author:      Davide Tarpini (https://github.com/Sparkhand)
 * Description: Similarly to example 1.2.4, this example shows how to draw two
 *              triangles with two different fragment shaders.
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
    const char* TITLE = "LearnOpenGL - 1.2.5 - Hello Triangle Exercise 3";
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

const char* FRAGMENT_SHADER_ORANGE_SRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char* FRAGMENT_SHADER_YELLOW_SRC =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";

/* --------- Additional functions declaration --------- */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* glfwSetup();
std::vector<unsigned int> createShaders();
std::vector<unsigned int> linkShaders(const unsigned int& vertexShader,
                                      const unsigned int& fragmentShaderOrange,
                                      const unsigned int& fragmentShaderYellow);
std::vector<unsigned int*> getVObjects();
void clearResources(unsigned int VBOs[],
                    unsigned int VAOs[],
                    unsigned int shaderProgramOrange,
                    unsigned int shaderProgramYellow);

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

    if (shaders.size() != 3)
    {
        std::cerr << "ERROR! Shaders vector doesn't have 3 elements" << std::endl;
        return -1;
    }

    unsigned int vertexShader = shaders[0];
    unsigned int fragmentShaderOrange = shaders[1];
    unsigned int fragmentShaderYellow = shaders[2];

    // Link shaders
    std::vector<unsigned int> shaderPrograms;
    try
    {
        shaderPrograms =
            linkShaders(vertexShader, fragmentShaderOrange, fragmentShaderYellow);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    if (shaderPrograms.size() != 2)
    {
        std::cerr << "ERROR! ShaderPrograms vector doesn't have 2 elements" << std::endl;
        return -1;
    }

    unsigned int shaderProgramOrange = shaderPrograms[0];
    unsigned int shaderProgramYellow = shaderPrograms[1];

    // Delete shaders after linking as they are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShaderOrange);
    glDeleteShader(fragmentShaderYellow);

    // Vertex data and buffer
    std::vector<unsigned int*> vObjects = getVObjects();

    if (vObjects.size() != 2)
    {
        std::cerr << "ERROR! VertexObjects vector doesn't have 2 elements" << std::endl;
        return -1;
    }

    unsigned int* VBOs = vObjects[0];
    unsigned int* VAOs = vObjects[1];

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

        // First triangle
        glUseProgram(shaderProgramOrange);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Second triangle
        glUseProgram(shaderProgramYellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // (GLFW) Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Once the render loop is finished, terminate GLFW and clear/free resources
    clearResources(VBOs, VAOs, shaderProgramOrange, shaderProgramYellow);
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

    // ORANGE
    unsigned int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderOrange, 1, &FRAGMENT_SHADER_ORANGE_SRC, NULL);
    glCompileShader(fragmentShaderOrange);

    // Check for errors in orange fragment shader compilation
    glGetShaderiv(fragmentShaderOrange, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderOrange, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaders.push_back(fragmentShaderOrange);
    }

    // YELLOW
    unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderYellow, 1, &FRAGMENT_SHADER_YELLOW_SRC, NULL);
    glCompileShader(fragmentShaderYellow);

    // Check for errors in yellow fragment shader compilation
    glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::FRAGMENT::ORANGE::COMPILATION_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaders.push_back(fragmentShaderYellow);
    }

    return shaders;
}

// Link shaders
// -----------------------------------------------------------------------------
std::vector<unsigned int> linkShaders(const unsigned int& vertexShader,
                                      const unsigned int& fragmentShaderOrange,
                                      const unsigned int& fragmentShaderYellow)
{
    std::vector<unsigned int> shaderPrograms;
    int success;
    char infoLog[512];

    // ORANGE
    // Link vertex and fragment shaders into a shader program
    unsigned int shaderProgramOrange = glCreateProgram();
    glAttachShader(shaderProgramOrange, vertexShader);
    glAttachShader(shaderProgramOrange, fragmentShaderOrange);
    glLinkProgram(shaderProgramOrange);

    // Check for errors in shader program linking
    glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgramOrange, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::ORANGE::LINKING_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaderPrograms.push_back(shaderProgramOrange);
    }

    // YELLOW
    // Link vertex and fragment shaders into a shader program
    unsigned int shaderProgramYellow = glCreateProgram();
    glAttachShader(shaderProgramYellow, vertexShader);
    glAttachShader(shaderProgramYellow, fragmentShaderYellow);
    glLinkProgram(shaderProgramYellow);

    // Check for errors in shader program linking
    glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderProgramYellow, 512, NULL, infoLog);
        throw std::runtime_error("ERROR::SHADER::PROGRAM::YELLOW::LINKING_FAILED\n" +
                                 std::string(infoLog));
    }
    else
    {
        shaderPrograms.push_back(shaderProgramYellow);
    }

    return shaderPrograms;
}

// Creation of vertex data and buffer objects, and binding
// -----------------------------------------------------------------------------
std::vector<unsigned int*> getVObjects()
{
    // Vertices
    const float FIRST_TRIANGLE_VERTICES[] = {
        // First triangle
        -0.9f,  -0.5f, 0.0f,  // left
        -0.0f,  -0.5f, 0.0f,  // right
        -0.45f, 0.5f,  0.0f,  // top
    };

    const float SECOND_TRIANGLE_VERTICES[] = {
        // Second triangle
        0.0f,  -0.5f, 0.0f,  // left
        0.9f,  -0.5f, 0.0f,  // right
        0.45f, 0.5f,  0.0f   // top
    };

    std::vector<unsigned int*> vObjects;

    // Vertex buffer object (VBO) and vertex array object (VAO) creation
    unsigned int* VBOs = new unsigned int(2);
    unsigned int* VAOs = new unsigned int(2);

    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // First triangle setup
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(FIRST_TRIANGLE_VERTICES),
                 FIRST_TRIANGLE_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Second triangle setup
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SECOND_TRIANGLE_VERTICES),
                 SECOND_TRIANGLE_VERTICES, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vObjects.push_back(VBOs);
    vObjects.push_back(VAOs);

    return vObjects;
}

// Clear resources at the end of render loop
// -----------------------------------------------------------------------------
void clearResources(unsigned int VBOs[],
                    unsigned int VAOs[],
                    unsigned int shaderProgramOrange,
                    unsigned int shaderProgramYellow)
{
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shaderProgramOrange);
    glDeleteProgram(shaderProgramYellow);
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