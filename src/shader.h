#ifndef INCLUDE_SRC_SHADER_H_
#define INCLUDE_SRC_SHADER_H_

unsigned int n_create_shader(const char *vertexPath, const char *fragmentPath);
void n_shader_set_uniform_i(unsigned int program_id, char *name, int value);
void n_shader_set_uniform_f(unsigned int program_id, char *name, float value);
void n_shader_set_uniform_b(unsigned int program_id, char *name, int value);
void n_shader_set_uniform_m4(unsigned int program_id, char *name, float *mat);

#endif // INCLUDE_SRC_SHADER_H_
