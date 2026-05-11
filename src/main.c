#include "camera.h"
#include "cglm/affine-post.h"
#include "cglm/affine.h"
#include "cglm/mat3.h"
#include "cglm/mat4.h"
#include "glad/glad.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <config.h>

struct InputState {
  float last_x;
  float last_y;
  int first_mouse;
};

struct AppConfig {
  const char *assets_path;
};

struct App {
  struct Window window;
  struct Camera camera;
  struct InputState input;
  struct AppConfig config;
  struct Texture texture;

  struct Shader cube_shader;
  struct Shader light_shader;

  struct Mesh cube;

  float delta_time;
  float last_frame;
};

static void app_destroy(struct App *app)
{
  n_mesh_destroy(&app->cube);
  n_shader_destroy(&app->cube_shader);
  n_texture_destroy(&app->texture);
  n_window_destroy(&app->window);
  glfwTerminate();
}

static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  struct App *app = glfwGetWindowUserPointer(window);

  if (app != NULL) {
    app->window.width = width;
    app->window.height = height;
  }

  glViewport(0, 0, width, height);
}

static void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
  struct App *app = glfwGetWindowUserPointer(window);
  float x_offset;
  float y_offset;

  if (app == NULL) {
    return;
  }

  if (app->input.first_mouse) {
    app->input.last_x = (float)x_pos;
    app->input.last_y = (float)y_pos;
    app->input.first_mouse = 0;
  }

  x_offset = (float)x_pos - app->input.last_x;
  y_offset = app->input.last_y - (float)y_pos;

  app->input.last_x = (float)x_pos;
  app->input.last_y = (float)y_pos;

  n_camera_process_mouse(&app->camera, x_offset, y_offset);
}

static void app_handle_input(struct App *app)
{
  GLFWwindow *window = app->window.self;
  const float camera_speed = 2.5f * app->delta_time;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    n_camera_move_forward(&app->camera, camera_speed);
  }

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    n_camera_move_forward(&app->camera, -camera_speed);
  }

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    n_camera_move_right(&app->camera, -camera_speed);
  }

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    n_camera_move_right(&app->camera, camera_speed);
  }

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    n_camera_move_up(&app->camera, camera_speed);
  }

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    n_camera_move_up(&app->camera, -camera_speed);
  }
}

static void app_update_time(struct App *app)
{
  float current_frame = (float)glfwGetTime();

  app->delta_time = current_frame - app->last_frame;
  app->last_frame = current_frame;
}

static bool app_init(struct App *app)
{
  app->config.assets_path = ASSETS_PATH;
  app->input.last_x = 400.0f;
  app->input.last_y = 300.0f;
  app->input.first_mouse = 1;
  app->delta_time = 0.0f;
  app->last_frame = 0.0f;
  app->texture.id = 0;

  app->cube_shader.id = 0;
  app->cube.vao = 0;
  app->cube.vbo = 0;
  app->cube.vertex_count = 0;

  app->light_shader.id = 0;

  n_camera_init(&app->camera);

  if (!n_init_window(&app->window, 800, 600, "noodle")) {
    return false;
  }

  glfwSetWindowUserPointer(app->window.self, app);
  glfwSetFramebufferSizeCallback(app->window.self, framebuffer_size_callback);
  glfwSetInputMode(app->window.self, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPosCallback(app->window.self, mouse_callback);

  if (!n_init_texture(&app->texture, ASSETS_PATH "textures/dirt.png")) {
    return false;
  }

  if (!n_shader_init(&app->cube_shader, ASSETS_PATH "shaders/cube_vert.glsl",
                     ASSETS_PATH "shaders/cube_frag.glsl")) {
    return false;
  }

  if (!n_shader_init(&app->light_shader, ASSETS_PATH "shaders/cube_vert.glsl",
                     ASSETS_PATH "shaders/light_frag.glsl")) {
    return false;
  }

  n_mesh_init_cube(&app->cube);

  n_shader_use(&app->cube_shader);

  // Texture unit 0 -> `tex`
  n_shader_set_uniform_i(&app->cube_shader, "tex", 0);

  glEnable(GL_DEPTH_TEST);
  return true;
}

static void app_render(struct App *app)
{
  mat4 view;
  mat4 proj;

  mat4 model_cube;
  mat3 model_cube_inv_T;
  mat4 model_light;

  vec3 light_pos = {1.2f, 1.0f, 2.0f};
  vec3 light_scale = {0.2f, 0.2f, 0.2f};
  vec3 light_color = {1.0f, 1.0f, 1.0f};

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Bind Cube texture
  n_texture_bind(&app->texture, 0);
  n_shader_use(&app->cube_shader);

  // MATH (view projection)
  n_camera_view_matrix(&app->camera, view);
  glm_perspective(glm_rad(45.0f),
                  (float)app->window.width / (float)app->window.height,
                  0.1f, 100.0f, proj);

  n_shader_set_uniform_m4(&app->cube_shader, "view", &view[0][0]);
  n_shader_set_uniform_m4(&app->cube_shader, "projection", &proj[0][0]);
  n_shader_set_uniform_v3(&app->cube_shader, "light_color", &light_color[0]);
  n_shader_set_uniform_v3(&app->cube_shader, "light_pos", &light_pos[0]);
  n_shader_set_uniform_v3(&app->cube_shader, "view_pos", &app->camera.pos[0]);

  // Cube math
  glm_mat4_identity(model_cube);
  mat3 m3;
  glm_mat4_pick3(model_cube, m3);
  glm_mat3_inv(m3, model_cube_inv_T);
  glm_mat3_transpose(model_cube_inv_T);
  n_shader_set_uniform_m4(&app->cube_shader, "model", &model_cube[0][0]);
  n_shader_set_uniform_m3(&app->cube_shader, "model_inv_T", &model_cube_inv_T[0][0]);

  // Render Cube
  n_mesh_draw(&app->cube);

  // Use light shader
  n_shader_use(&app->light_shader);

  n_shader_set_uniform_m4(&app->light_shader, "view", &view[0][0]);
  n_shader_set_uniform_m4(&app->light_shader, "projection", &proj[0][0]);

  glm_mat4_identity(model_light);
  glm_translated(model_light, light_pos);
  glm_scale(model_light, light_scale);

  // Light math
  n_shader_set_uniform_m4(&app->light_shader, "model", &model_light[0][0]);

  // Render same mesh for light
  n_mesh_draw(&app->cube);
}

int main(void)
{
  struct App app;

  if (!app_init(&app)) {
    app_destroy(&app);
    return 1;
  }

  while (!glfwWindowShouldClose(app.window.self)) {
    app_update_time(&app);
    app_handle_input(&app);
    app_render(&app);

    glfwSwapBuffers(app.window.self);
    glfwWaitEvents();
  }

  app_destroy(&app);
  return 0;
}
