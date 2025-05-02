#include "cglm/cam.h"
#include "glad/glad.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

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
        unsigned int vao, vbo;

        mat4 model, view, projection;
        vec3 axis, camera_pos;
        float axis_arr[] = {0.5f, 1.0f, 0.0f};
        float camera_pos_arr[] = {0.0f, 0.0f, -3.0f};

        n_init_window(&window, 800, 600, "noodle");
        n_init_texture(&texture, "assets/textures/container.jpg");
        shader = n_create_shader("assets/shaders/simpleVert.glsl",
                                 "assets/shaders/simpleFrag.glsl");

        // clang-format off
        float vertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
        };
        // clang-format on

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        // glGenBuffers(1, &ebo);

        glBindVertexArray(vao);

        // Setup VBO
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

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
        n_shader_set_uniform_i(shader, "v_texture", 0);

        glEnable(GL_DEPTH_TEST);

        while (!glfwWindowShouldClose(window.self)) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture.id);

                glUseProgram(shader);

                glm_vec3_make(camera_pos_arr, camera_pos);
                glm_translate_make(view, camera_pos);

                glm_perspective(glm_rad(45.0f),
                                (float)window.width / (float)window.height,
                                0.1f, 100.0f, projection);

                n_shader_set_uniform_m4(shader, "view", &view[0][0]);
                n_shader_set_uniform_m4(shader, "projection",
                                        &projection[0][0]);

                glBindVertexArray(vao);

                /* AGHH */
                glm_vec3_make(axis_arr, axis);
                glm_rotate_make(model, (float)glfwGetTime() * glm_rad(-55.0f),
                                axis);
                n_shader_set_uniform_m4(shader, "model", &model[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, 36);

                handle_input(window.self);
                glfwSwapBuffers(window.self);
                glfwPollEvents();
        }

        glfwTerminate();
        return 0;
}
