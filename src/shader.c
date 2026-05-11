#include "shader.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_file_content(const char *file_path)
{
  char *buffer = NULL;
  long length;
  size_t bytes_read;
  FILE *file = fopen(file_path, "rb");

  if (NULL == file) {
    printf("Failed to load shader file: %s\n", file_path);
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  length = ftell(file);
  fseek(file, 0, SEEK_SET);
  buffer = malloc(length + 1);

  if (NULL == buffer) {
    printf("Failed to allocate %ld bytes for buffer.\n",
           length + 1);
    fclose(file);
    return NULL;
  }

  bytes_read = fread(buffer, 1, length, file);
  if (bytes_read != (size_t)length) {
    printf("Failed to read shader file: %s\n", file_path);
    free(buffer);
    fclose(file);
    return NULL;
  }
  buffer[length] = '\0';
  fclose(file);
  return buffer;
}

static bool check_compile_errors(unsigned int shader)
{
  int success;
  char infoLog[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    printf("Error compiling shader: %s\n", infoLog);
    return false;
  }

  return true;
}

static bool check_linking_errors(unsigned int shader_program)
{
  int success;
  char info_log[512];

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shader_program, 512, NULL, info_log);
    printf("Error linking shader: %s\n", info_log);
    return false;
  }

  return true;
}

bool n_shader_init(struct Shader *shader, const char *vertex_path,
                   const char *fragment_path)
{
  const char *vertex_source = get_file_content(vertex_path);
  const char *fragment_source = get_file_content(fragment_path);

  unsigned int vertex_shader;
  unsigned int fragment_shader;
  if (shader == NULL) {
    printf("Error in function n_shader_init(): shader is NULL\n");
    free((char *)vertex_source);
    free((char *)fragment_source);
    return false;
  }

  shader->id = 0;

  if (vertex_source == NULL || fragment_source == NULL) {
    free((char *)vertex_source);
    free((char *)fragment_source);
    return false;
  }

  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_source, NULL);
  glCompileShader(vertex_shader);
  if (!check_compile_errors(vertex_shader)) {
    glDeleteShader(vertex_shader);
    free((char *)vertex_source);
    free((char *)fragment_source);
    return false;
  }

  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_source, NULL);
  glCompileShader(fragment_shader);
  if (!check_compile_errors(fragment_shader)) {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    free((char *)vertex_source);
    free((char *)fragment_source);
    return false;
  }

  shader->id = glCreateProgram();
  glAttachShader(shader->id, vertex_shader);
  glAttachShader(shader->id, fragment_shader);
  glLinkProgram(shader->id);
  if (!check_linking_errors(shader->id)) {
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    n_shader_destroy(shader);
    free((char *)vertex_source);
    free((char *)fragment_source);
    return false;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  free((char *)vertex_source);
  free((char *)fragment_source);

  return true;
}

void n_shader_use(const struct Shader *shader)
{
  glUseProgram(shader->id);
}

void n_shader_destroy(struct Shader *shader)
{
  if (shader != NULL && shader->id != 0) {
    glDeleteProgram(shader->id);
    shader->id = 0;
  }
}

void n_shader_set_uniform_i(const struct Shader *shader, const char *name, int value)
{
  glUniform1i(glGetUniformLocation(shader->id, name), value);
}
void n_shader_set_uniform_f(const struct Shader *shader, const char *name, float value)
{
  glUniform1f(glGetUniformLocation(shader->id, name), value);
}
void n_shader_set_uniform_b(const struct Shader *shader, const char *name, int value)
{
  glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void n_shader_set_uniform_m4(const struct Shader *shader, const char *name,
                             const float *mat)
{
  glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                     mat);
}

void n_shader_set_uniform_m3(const struct Shader *shader, const char *name,
                             const float *mat)
{
  glUniformMatrix3fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE,
                     mat);
}

void n_shader_set_uniform_v3(const struct Shader *shader, const char *name, const float *vec)
{
  // `count` represents how many vec3s, mat4s, etc. will be passed in
  glUniform3fv(glGetUniformLocation(shader->id, name), 1, vec);
}
