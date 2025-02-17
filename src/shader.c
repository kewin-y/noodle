#include "shader.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

static char *get_file_content(const char *file_path)
{
        char *buffer = NULL;
        long length;
        FILE *file = fopen(file_path, "rb");

        if (NULL == file) {
                printf("Failed to load shader file: %s\n", file_path);
                exit(1);
        }

        fseek(file, 0, SEEK_END);
        length = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc(length + 1);

        if (NULL == buffer) {
                printf("Failed to allocate %ld bytes for buffer.\n",
                       length + 1);
                exit(1);
        }

        fread(buffer, 1, length, file);
        *(buffer + length) = '\0';

        return buffer;
}

static void check_compile_errors(unsigned int shader) {
        int success;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                printf("Error compiling shader: %s\n", infoLog);
                exit(1);
        }
}

static void check_linking_errors(unsigned int shader_program) {
        int success;
        char info_log[512];

        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);

        if (!success) {
                glGetProgramInfoLog(shader_program, 512, NULL, info_log);
                printf("Error linking shader: %s\n", info_log);
                exit(1);
        }
}

unsigned int create_shader(const char *vertex_path, const char *fragment_path)
{
        const char *vertex_source = get_file_content(vertex_path);
        const char *fragment_source = get_file_content(fragment_path);

        unsigned int vertex_shader;
        unsigned int fragment_shader;
        unsigned int shader_program;

        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_source, NULL);
        glCompileShader(vertex_shader);
        check_compile_errors(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_source, NULL);
        glCompileShader(fragment_shader);
        check_compile_errors(fragment_shader);

        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);
        check_linking_errors(shader_program);

        free((char *)vertex_source);
        free((char *)fragment_source);

        return shader_program;
}
void shader_set_uniform_i(unsigned int program_id, char *name, int value) {
        glUniform1i(glGetUniformLocation(program_id, name), value);
}
void shader_set_uniform_f(unsigned int program_id, char *name, float value) {
        glUniform1f(glGetUniformLocation(program_id, name), value);
}
void shader_set_uniform_b(unsigned int program_id, char *name, int value) {
        glUniform1i(glGetUniformLocation(program_id, name), value);
}
