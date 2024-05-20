/******************************************************************************
 * File:        1.4.1.textures.cpp
 * Author:      Davide Tarpini (https://github.com/Sparkhand)
 * Description: This example shows how to load and display a texture on a
 *              rectangle. The texture is loaded from a file and applied to the
 *              rectangle using the texture coordinates.
 *****************************************************************************/

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <learnopengl/shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <string>
#include <vector>

/* --------- Global vars and constants --------- */

// Window/Screen properties
const struct WINDOW_PROPS
{
    // Screen
    const unsigned int SCR_WIDTH = 800;
    const unsigned int SCR_HEIGHT = 600;
    // Window title
    const char* TITLE = "LearnOpenGL - 1.4.1 - Textures";
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
unsigned int loadTexture(std::string path);
void clearResources(unsigned int VBO, unsigned int VAO, unsigned int EBO);

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
    Shader ourShader("1.4.1.textures.vs", "1.4.1.textures.fs");

    // Vertex data and buffer
    std::vector<unsigned int> vObjects = getVObjects();

    if (vObjects.size() != 3)
    {
        std::cerr << "ERROR! VertexObjects vector doesn't have 3 elements" << std::endl;
        return -1;
    }

    unsigned int VBO = vObjects[0];
    unsigned int VAO = vObjects[1];
    unsigned int EBO = vObjects[2];

    // OPTIONAL: Wireframe mode (unc/comment to toggle)
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Load and create a texture
    unsigned int texture;
    try
    {
        texture = loadTexture("resources/textures/container.jpg");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        // Input handling
        processInput(window);

        // Render
        glClearColor(WINDOW_PROPS.CLEAR_COLOR.r, WINDOW_PROPS.CLEAR_COLOR.g,
                     WINDOW_PROPS.CLEAR_COLOR.b, WINDOW_PROPS.CLEAR_COLOR.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw container
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // (GLFW) Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Once the render loop is finished, terminate GLFW and clear/free resources
    clearResources(VBO, VAO, EBO);
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
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    // Indices
    const unsigned int INDICES[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    std::vector<unsigned int> vObjects;

    // Create VBO and VAO, and this time EBO (Element Buffer Object)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO first, then VBO and EBO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VAO
    glBindVertexArray(0);

    return {VBO, VAO, EBO};
}

// Load texture from file
// -----------------------------------------------------------------------------
unsigned int loadTexture(std::string path)
{
    // Texture creation and loading
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // From here on, all texture functions will modify this texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);  // (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                     data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        throw std::runtime_error("Failed to load texture");
    }

    stbi_image_free(data);

    return texture;
}

// Clear resources at the end of render loop
// -----------------------------------------------------------------------------
void clearResources(unsigned int VBO, unsigned int VAO, unsigned int EBO)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
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