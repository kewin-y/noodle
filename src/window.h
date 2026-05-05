#ifndef INCLUDE_SRC_WINDOW_H
#define INCLUDE_SRC_WINDOW_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

struct Window {
  int width;
  int height;
  const char *name;
  GLFWwindow *self;
};

bool n_init_window(struct Window *win, int width, int height, const char *name);
void n_window_destroy(struct Window *win);

#endif
