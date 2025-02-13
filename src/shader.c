#include "shader.h"
#include "glad/glad.h"
#include <stdio.h>
#include <stdlib.h>

static char *getFileContent(const char *filePath)
{
        char *buffer = NULL;
        long length;
        FILE *file = fopen(filePath, "rb");

        if (NULL == file) {
                printf("Failed to load shader file: %s\n", filePath);
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

static void checkCompileErrors(unsigned int shader) {
        int success;
        char infoLog[512];

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                printf("Error compiling shader: %s\n", infoLog);
                exit(1);
        }
}

static void checkLinkingErrors(unsigned int shaderProgram) {
        int success;
        char infoLog[512];

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

        if (!success) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
                printf("Error linking shader: %s\n", infoLog);
                exit(1);
        }
}

unsigned int createShader(const char *vertexPath, const char *fragmentPath)
{
        const char *vertexShaderCode = getFileContent(vertexPath);
        const char *fragmentShaderCode = getFileContent(fragmentPath);

        unsigned int vertexShader;
        unsigned int fragmentShader;
        unsigned int shaderProgram;

        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader);

        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);
        checkCompileErrors(fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        checkLinkingErrors(shaderProgram);

        free((char *)vertexShaderCode);
        free((char *)fragmentShaderCode);

        return shaderProgram;
}
