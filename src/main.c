#include "glad/glad.h"
#include "window.h"
#include "shader.h"
#include "texture.h"
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
        struct Texture texture;
        unsigned int shader;

        init_window(&window, 800, 600, "noodle");
        init_texture(&texture, "assets/textures/container.jpg");
        shader = create_shader("assets/shaders/simpleVert.glsl",
                                            "assets/shaders/simpleFrag.glsl");

        // clang-format off
        float vertices[] = {
                -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f,     0.0f, 1.0f
        };

        int indices[] = {
                0, 1, 3,
                1, 2, 3
        };
        // clang-format on

        unsigned int vao;
        unsigned int vbo;
        unsigned int ebo;

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        // Setup VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        // Setup EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                     GL_STATIC_DRAW);

        // a_pos
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        // a_tex_coord
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                              (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glUseProgram(shader);
        shader_set_uniform_i(shader, "v_texture", 0);

        while (!glfwWindowShouldClose(window.self)) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture.id);

                glUseProgram(shader);
                glBindVertexArray(vao);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                handle_input(window.self);
                glfwSwapBuffers(window.self);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
