#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

#define TRUE (1 == 1)
#define FALSE (!TRUE)

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void handleInput(GLFWwindow *window);

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, TRUE);
        }
}

int main()
{
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window =
            glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "noodle", NULL, NULL);

        if (NULL == window) {
                printf("Error: Failed to create window.");
                glfwTerminate();
                return -1;
        }

        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                printf("Error: Failed to initialize GLAD.");
                return -1;
        }

        while (!glfwWindowShouldClose(window)) {
                handleInput(window);

                glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glfwPollEvents();
                glfwSwapBuffers(window);
        }

        glfwTerminate();
        return 0;
}
