#include "camera.h"
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
  struct Shader shader;
  struct Mesh cube;
  float delta_time;
  float last_frame;
};

static void app_destroy(struct App *app)
{
  n_mesh_destroy(&app->cube);
  n_shader_destroy(&app->shader);
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
  app->shader.id = 0;
  app->cube.vao = 0;
  app->cube.vbo = 0;
  app->cube.vertex_count = 0;

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

  if (!n_shader_init(&app->shader, ASSETS_PATH "shaders/simpleVert.glsl",
                     ASSETS_PATH "shaders/simpleFrag.glsl")) {
    return false;
  }

  n_mesh_init_cube(&app->cube);

  n_shader_use(&app->shader);
  n_shader_set_uniform_i(&app->shader, "v_texture", 0);

  glEnable(GL_DEPTH_TEST);
  return true;
}

static void app_render(struct App *app)
{
  mat4 model;
  mat4 view;
  mat4 projection;

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  n_texture_bind(&app->texture, 0);
  n_shader_use(&app->shader);

  n_camera_view_matrix(&app->camera, view);
  glm_perspective(glm_rad(45.0f),
                  (float)app->window.width / (float)app->window.height,
                  0.1f, 100.0f, projection);

  n_shader_set_uniform_m4(&app->shader, "view", &view[0][0]);
  n_shader_set_uniform_m4(&app->shader, "projection", &projection[0][0]);

  glm_mat4_identity(model);
  n_shader_set_uniform_m4(&app->shader, "model", &model[0][0]);

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
