#include "glad/glad.h"
#include "shader.h"
#include "window.h"
#include <GLFW/glfw3.h>

// Declarations
static void handle_input(GLFWwindow *window);

// Definitions
static void handle_input(GLFWwindow *window)
{
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, 1);
        }
}

int main()
{
        struct Window window;
        init_window(&window, 800, 600, "noodle");

        unsigned int shader = create_shader("assets/shaders/simpleVert.glsl",
                                            "assets/shaders/simpleFrag.glsl");

        // clang-format off
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,
                 0.5f, -0.5f, 0.0f,
                 0.0f,  0.5f, 0.0f
        };
        // clang-format on

        unsigned int vao;
        unsigned int vbo;
        // unsigned int ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        // glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        while (!glfwWindowShouldClose(window.self)) {
                glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glUseProgram(shader);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, 3);

                handle_input(window.self);
                glfwSwapBuffers(window.self);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
