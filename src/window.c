#include "window.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

static void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
        glViewport(0, 0, width, height);
}

struct Window initWindow(int width, int height, const char *name)
{
        struct Window win;

        win.width = width;
        win.height = height;
        win.name = name;

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        win.self = glfwCreateWindow(width, height, name, NULL, NULL);

        if (NULL == win.self) {
                printf("Error: Failed to create window.");
                glfwTerminate();
                exit(1);
        }

        glfwMakeContextCurrent(win.self);
        glfwSetFramebufferSizeCallback(win.self, framebufferSizeCallback);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
                printf("Error: Failed to initialize GLAD.");
                exit(1);
        }

        return win;
}
