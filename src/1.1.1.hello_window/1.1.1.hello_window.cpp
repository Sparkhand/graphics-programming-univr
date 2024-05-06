#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

/* Global vars and constants */

// Window-related
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const char *WINDOW_TITLE = "LearnOpenGL - 1.1.1 - Hello Window";
const struct CLEAR_COLOR
{
    float r = 0.2f;
    float g = 0.3f;
    float b = 0.3f;
    float a = 1.0f;
} clear_color;

/* Additional functions declaration */
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

/* Main */
int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // MacOS compatibility code
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, WINDOW_TITLE, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL funciton pointers (GLAD init)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Render
        glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // (GLFW) Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Once the render loop is finished, terminate GLFW, clearing any resources allocated by GLFW
    glfwTerminate();
    return 0;
}

/* Additional functions definition */

// User input handling
// -----------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    // Close the window on ESC
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Resize the viewport when the window is resized
// -----------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}