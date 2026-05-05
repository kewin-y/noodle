#ifndef INCLUDE_SRC_MESH_H
#define INCLUDE_SRC_MESH_H

struct Mesh {
  unsigned int vao;
  unsigned int vbo;
  int vertex_count;
};

void n_mesh_init_cube(struct Mesh *mesh);
void n_mesh_draw(const struct Mesh *mesh);
void n_mesh_destroy(struct Mesh *mesh);

#endif
