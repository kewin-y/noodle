#include "window.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

static void n_fb_size_callback(GLFWwindow *window, int width, int height)
{
  struct Window *win = glfwGetWindowUserPointer(window);

  if (win != NULL) {
    win->width = width;
    win->height = height;
  }

  glViewport(0, 0, width, height);
}

bool n_init_window(struct Window *win, int width, int height, const char *name)
{
  if (NULL == win) {
    printf("Error in function n_init_window(): win is NULL\n");
    return false;
  }

  win->width = width;
  win->height = height;
  win->name = name;

  if (!glfwInit()) {
    printf("Error: Failed to initialize GLFW.\n");
    return false;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  win->self = glfwCreateWindow(width, height, name, NULL, NULL);

  if (NULL == win->self) {
    printf("Error: Failed to create window.\n");
    glfwTerminate();
    return false;
  }

  glfwMakeContextCurrent(win->self);
  glfwSetWindowUserPointer(win->self, win);
  glfwSetFramebufferSizeCallback(win->self, n_fb_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Error: Failed to initialize GLAD.\n");
    n_window_destroy(win);
    return false;
  }

  return true;
}

void n_window_destroy(struct Window *win)
{
  if (win != NULL && win->self != NULL) {
    glfwDestroyWindow(win->self);
    win->self = NULL;
  }
}
