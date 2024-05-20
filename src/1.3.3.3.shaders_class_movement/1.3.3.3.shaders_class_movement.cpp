/******************************************************************************
 * File:        1.3.3.3.shaders_class_movement.cpp
 * Author:      Davide Tarpini (https://github.com/Sparkhand)
 * Description: This example shows how to use the Shader class to simplify the
 *              shader management and how to move the triangle on the x-axis
 *              over time by using a uniform variable.
 *****************************************************************************/

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <learnopengl/shader_s.h>

#include <cmath>
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
    const char* TITLE = "LearnOpenGL - 1.3.3 - Shaders Class";
    // Clear color
    const struct CLEAR_COLOR
    {
        float r = 0.2f;
        float g = 0.3f;
        float b = 0.3f;
        float a = 1.0f;
    } CLEAR_COLOR;
} WINDOW_PROPS;

/* --------- Additional functions declaration --------- */
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLFWwindow* glfwSetup();
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

    // Compile and link shaders
    Shader ourShader("1.3.3.3.shaders_class_movement.vs",
                     "1.3.3.3.shaders_class_movement.fs");

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

        // Draw the triangle
        double timeValue = glfwGetTime();
        float xOffset = static_cast<float>(sin(timeValue) / 2.0);
        ourShader.setFloat("xOffset", xOffset);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

// Creation of vertex data and buffer objects, and binding
// -----------------------------------------------------------------------------
std::vector<unsigned int> getVObjects()
{
    // Vertices
    const float VERTICES[] = {
        // positions         // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // top
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

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VAO
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
void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}