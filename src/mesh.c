#include "mesh.h"

#include "glad/glad.h"
#include <stddef.h>

void n_mesh_init_cube(struct Mesh *mesh)
{
  // clang-format off
  float vertices[] = {
     -0.5f,  -0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  0.0f,  0.0f,
      0.5f,  -0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  1.0f,  0.0f,
      0.5f,   0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  1.0f,  1.0f,
      0.5f,   0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  1.0f,  1.0f,
     -0.5f,   0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  0.0f,  1.0f,
     -0.5f,  -0.5f,  -0.5f,   0.0f,   0.0f,  -1.0f,  0.0f,  0.0f,

     -0.5f,  -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  0.0f,  0.0f,
      0.5f,  -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  1.0f,  0.0f,
      0.5f,   0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  1.0f,  1.0f,
      0.5f,   0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  1.0f,  1.0f,
     -0.5f,   0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  0.0f,  1.0f,
     -0.5f,  -0.5f,   0.5f,   0.0f,   0.0f,   1.0f,  0.0f,  0.0f,

     -0.5f,   0.5f,   0.5f,  -1.0f,   0.0f,   0.0f,  1.0f,  0.0f,
     -0.5f,   0.5f,  -0.5f,  -1.0f,   0.0f,   0.0f,  1.0f,  1.0f,
     -0.5f,  -0.5f,  -0.5f,  -1.0f,   0.0f,   0.0f,  0.0f,  1.0f,
     -0.5f,  -0.5f,  -0.5f,  -1.0f,   0.0f,   0.0f,  0.0f,  1.0f,
     -0.5f,  -0.5f,   0.5f,  -1.0f,   0.0f,   0.0f,  0.0f,  0.0f,
     -0.5f,   0.5f,   0.5f,  -1.0f,   0.0f,   0.0f,  1.0f,  0.0f,

      0.5f,   0.5f,   0.5f,   1.0f,   0.0f,   0.0f,  1.0f,  0.0f,
      0.5f,   0.5f,  -0.5f,   1.0f,   0.0f,   0.0f,  1.0f,  1.0f,
      0.5f,  -0.5f,  -0.5f,   1.0f,   0.0f,   0.0f,  0.0f,  1.0f,
      0.5f,  -0.5f,  -0.5f,   1.0f,   0.0f,   0.0f,  0.0f,  1.0f,
      0.5f,  -0.5f,   0.5f,   1.0f,   0.0f,   0.0f,  0.0f,  0.0f,
      0.5f,   0.5f,   0.5f,   1.0f,   0.0f,   0.0f,  1.0f,  0.0f,

     -0.5f,  -0.5f,  -0.5f,   0.0f,  -1.0f,   0.0f,  0.0f,  1.0f,
      0.5f,  -0.5f,  -0.5f,   0.0f,  -1.0f,   0.0f,  1.0f,  1.0f,
      0.5f,  -0.5f,   0.5f,   0.0f,  -1.0f,   0.0f,  1.0f,  0.0f,
      0.5f,  -0.5f,   0.5f,   0.0f,  -1.0f,   0.0f,  1.0f,  0.0f,
     -0.5f,  -0.5f,   0.5f,   0.0f,  -1.0f,   0.0f,  0.0f,  0.0f,
     -0.5f,  -0.5f,  -0.5f,   0.0f,  -1.0f,   0.0f,  0.0f,  1.0f,

     -0.5f,   0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,  0.0f,  1.0f,
      0.5f,   0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,  1.0f,  1.0f,
      0.5f,   0.5f,   0.5f,   0.0f,   1.0f,   0.0f,  1.0f,  0.0f,
      0.5f,   0.5f,   0.5f,   0.0f,   1.0f,   0.0f,  1.0f,  0.0f,
     -0.5f,   0.5f,   0.5f,   0.0f,   1.0f,   0.0f,  0.0f,  0.0f,
     -0.5f,   0.5f,  -0.5f,   0.0f,   1.0f,   0.0f,  0.0f,  1.0f
  };
  // clang-format on

  mesh->vertex_count = 36;
  const int stride = 8 * sizeof(float);

  glGenVertexArrays(1, &mesh->vao);
  glGenBuffers(1, &mesh->vbo);

  glBindVertexArray(mesh->vao);

  glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride,
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void n_mesh_draw(const struct Mesh *mesh)
{
  glBindVertexArray(mesh->vao);
  glDrawArrays(GL_TRIANGLES, 0, mesh->vertex_count);
}

void n_mesh_destroy(struct Mesh *mesh)
{
  if (mesh->vbo != 0) {
    glDeleteBuffers(1, &mesh->vbo);
    mesh->vbo = 0;
  }
  if (mesh->vao != 0) {
    glDeleteVertexArrays(1, &mesh->vao);
    mesh->vao = 0;
  }
  mesh->vertex_count = 0;
}
