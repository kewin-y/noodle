#ifndef INCLUDE_SRC_SHADER_H_
#define INCLUDE_SRC_SHADER_H_

unsigned int createShader(const char *vertexPath, const char *fragmentPath);
void shaderSetIntUniform(unsigned int programID, char *name, int value);
void shaderSetFloatUniform(unsigned int programID, char *name, int value);
void shaderSetBoolUniform(unsigned int programID, char *name, int value);

#endif // INCLUDE_SRC_SHADER_H_
