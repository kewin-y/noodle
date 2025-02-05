#include "glad/glad.h"
#include "shader.h"
#include "window.h"
#include <GLFW/glfw3.h>

// Declarations
static void handleInput(GLFWwindow *window);

// Definitions
static void handleInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, 1);
        }
}

int main()
{
        struct Window window;

        window = initWindow(800, 600, "noodle");

        unsigned int shader = createShader("assets/shaders/simpleFrag.glsl",
                                           "assets/shaders/simpleVert.glsl");

        while (!glfwWindowShouldClose(window.self)) {
                handleInput(window.self);

                glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glfwPollEvents();
                glfwSwapBuffers(window.self);
        }

        glfwTerminate();
        return 0;
}
