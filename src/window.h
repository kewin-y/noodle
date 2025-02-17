#ifndef INCLUDE_SRC_WINDOW_H_
#define INCLUDE_SRC_WINDOW_H_

#include "glad/glad.h"
#include <GLFW/glfw3.h>

struct Window {
        int width;
        int height;
        const char *name;
        GLFWwindow *self;
};

void init_window(struct Window *win, int width, int height, const char *name);

#endif // INCLUDE_SRC_WINDOW_H_
