#ifndef INCLUDE_SRC_MESH_H
#define INCLUDE_SRC_MESH_H

#include <stdbool.h>

struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  int vertex_count;
};

// TODO: remove the ability to supply `with_tex_coord``
// Ideally, the mesh should have normals, uv coords, etc. etc.
// The shader will accomdate for that
void n_mesh_init_cube(struct Mesh *mesh, bool with_tex_coord);
void n_mesh_draw(const struct Mesh *mesh);
void n_mesh_destroy(struct Mesh *mesh);

#endif
