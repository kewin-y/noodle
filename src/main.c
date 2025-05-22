#include "glad/glad.h"
#include "shader.h"
#include "texture.h"
#include "window.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>

// GLOBALS
vec3 camera_pos = {0.0f, 0.0f, 3.0f};
vec3 camera_front = {0.0f, 0.0f, -1.0f};
vec3 camera_up = {0.0f, 1.0f, 0.0f};
vec3 camera_center = {0.0f, 0.0f, 0.0f};

float last_x = 400.0f, last_y = 300.0f;
float pitch = 0.0f, yaw = -90.0f;
int first_mouse = 1;

float delta_time = 0.0f;
float last_frame = 0.0f;

static void handle_input(GLFWwindow *win)
{
        const float camera_speed = 2.5f * delta_time;

        vec3 right, forward;

        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(win, 1);
        }

        glm_vec3_cross(camera_front, camera_up, right);
        glm_vec3_normalize(right);
        glm_vec3_cross(camera_up, right, forward);
        glm_vec3_normalize(forward);

        if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(forward, camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }

        if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(forward, -camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }

        if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(right, -camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }

        if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(right, camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }

        if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(camera_up, camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }

        if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
                vec3 tmp;
                glm_vec3_scale(camera_up, -camera_speed, tmp);
                glm_vec3_add(camera_pos, tmp, camera_pos);
        }
}

static void mouse_callback(GLFWwindow *win, double x_pos, double y_pos)
{
        if (first_mouse) {
                last_x = x_pos;
                last_y = y_pos;
                first_mouse = 0;
        }

        float x_offset = x_pos - last_x;
        float y_offset = last_y - y_pos; // As (0,0) is in the top-left corner
        const float sensitivity = 0.2f;

        vec3 direction;

        last_x = x_pos;
        last_y = y_pos;

        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch += y_offset;

        if (pitch > 89.0f)
                pitch = 89.0f;
        if (pitch < -89.0f)
                pitch = -89.0f;

        direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
        direction[1] = sin(glm_rad(pitch));
        direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));

        glm_normalize_to(direction, camera_front);
}

int main()
{
        struct Window win;
        struct Texture texture;

        unsigned int shader;

        unsigned int vao, vbo;

        mat4 model, view, projection;

        float current_frame;

        n_init_window(&win, 800, 600, "noodle");

        // AWFUL
        glfwSetInputMode(win.self, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(win.self, mouse_callback);

        n_init_texture(&texture, "assets/textures/dirt.png");
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

        while (!glfwWindowShouldClose(win.self)) {
                glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                current_frame = glfwGetTime();
                delta_time = current_frame - last_frame;
                last_frame = current_frame;

                // printf("delta_time=%f\n", delta_time);

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, texture.id);

                glUseProgram(shader);

                glm_vec3_print(camera_front, stdout);
                glm_vec3_add(camera_pos, camera_front, camera_center);
                glm_lookat(camera_pos, camera_center, camera_up, view);

                glm_perspective(glm_rad(45.0f),
                                (float)win.width / (float)win.height,
                                0.1f, 100.0f, projection);

                n_shader_set_uniform_m4(shader, "view", &view[0][0]);
                n_shader_set_uniform_m4(shader, "projection",
                                        &projection[0][0]);

                glBindVertexArray(vao);

                /* AGHH */
                glm_mat4_identity(model);
                n_shader_set_uniform_m4(shader, "model", &model[0][0]);

                glDrawArrays(GL_TRIANGLES, 0, 36);

                handle_input(win.self);
                glfwSwapBuffers(win.self);
                glfwWaitEvents();
        }

        glfwTerminate();
        return 0;
}
