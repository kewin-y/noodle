#ifndef INCLUDE_SRC_SHADER_H_
#define INCLUDE_SRC_SHADER_H_

unsigned int create_shader(const char *vertexPath, const char *fragmentPath);
void shader_set_uniform_i(unsigned int programID, char *name, int value);
void shader_set_uniform_f(unsigned int programID, char *name, float value);
void shader_set_uniform_b(unsigned int programID, char *name, int value);

#endif // INCLUDE_SRC_SHADER_H_
