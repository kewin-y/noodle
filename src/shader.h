#ifndef INCLUDE_SRC_SHADER_H
#define INCLUDE_SRC_SHADER_H

#include <stdbool.h>

struct Shader {
  unsigned int id;
};

bool n_shader_init(struct Shader *shader, const char *vertex_path,
                   const char *fragment_path);
void n_shader_use(const struct Shader *shader);
void n_shader_destroy(struct Shader *shader);
void n_shader_set_uniform_i(const struct Shader *shader, const char *name, int value);
void n_shader_set_uniform_f(const struct Shader *shader, const char *name, float value);
void n_shader_set_uniform_b(const struct Shader *shader, const char *name, int value);
void n_shader_set_uniform_m4(const struct Shader *shader, const char *name,
                             const float *mat);
void n_shader_set_uniform_m3(const struct Shader *shader, const char *name,
                             const float *mat);
void n_shader_set_uniform_v3(const struct Shader *shader, const char *name, const float *vec);

#endif
