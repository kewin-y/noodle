#include "camera.h"

#include <math.h>

static void n_camera_update_front(struct Camera *camera)
{
  vec3 direction;

  direction[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
  direction[1] = sinf(glm_rad(camera->pitch));
  direction[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));

  glm_normalize_to(direction, camera->front);
}

void n_camera_init(struct Camera *camera)
{
  glm_vec3_copy((vec3){0.0f, 0.0f, 3.0f}, camera->pos);
  glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
  glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);
  camera->yaw = -90.0f;
  camera->pitch = 0.0f;
}

void n_camera_move_forward(struct Camera *camera, float amount)
{
  vec3 right;
  vec3 forward;
  vec3 delta;

  glm_vec3_cross(camera->front, camera->up, right);
  glm_vec3_normalize(right);
  glm_vec3_cross(camera->up, right, forward);
  glm_vec3_normalize(forward);

  glm_vec3_scale(forward, amount, delta);
  glm_vec3_add(camera->pos, delta, camera->pos);
}

void n_camera_move_right(struct Camera *camera, float amount)
{
  vec3 right;
  vec3 delta;

  glm_vec3_cross(camera->front, camera->up, right);
  glm_vec3_normalize(right);

  glm_vec3_scale(right, amount, delta);
  glm_vec3_add(camera->pos, delta, camera->pos);
}

void n_camera_move_up(struct Camera *camera, float amount)
{
  vec3 delta;

  glm_vec3_scale(camera->up, amount, delta);
  glm_vec3_add(camera->pos, delta, camera->pos);
}

void n_camera_process_mouse(struct Camera *camera, float x_offset, float y_offset)
{
  const float sensitivity = 0.2f;

  camera->yaw += x_offset * sensitivity;
  camera->pitch += y_offset * sensitivity;

  if (camera->pitch > 89.0f) {
    camera->pitch = 89.0f;
  }
  if (camera->pitch < -89.0f) {
    camera->pitch = -89.0f;
  }

  n_camera_update_front(camera);
}

void n_camera_view_matrix(struct Camera *camera, mat4 out)
{
  vec3 center;

  glm_vec3_add(camera->pos, camera->front, center);
  glm_lookat(camera->pos, center, camera->up, out);
}
