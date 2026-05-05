#ifndef INCLUDE_SRC_CAMERA_H
#define INCLUDE_SRC_CAMERA_H

#include <cglm/cglm.h>

struct Camera {
  vec3 pos;
  vec3 front;
  vec3 up;
  float yaw;
  float pitch;
};

void n_camera_init(struct Camera *camera);
void n_camera_move_forward(struct Camera *camera, float amount);
void n_camera_move_right(struct Camera *camera, float amount);
void n_camera_move_up(struct Camera *camera, float amount);
void n_camera_process_mouse(struct Camera *camera, float x_offset, float y_offset);
void n_camera_view_matrix(struct Camera *camera, mat4 out);

#endif
