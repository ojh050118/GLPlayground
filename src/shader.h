#ifndef GLPLAYGROUND_SHADER_H
#define GLPLAYGROUND_SHADER_H


#include "glad/glad.h"

class Shader
{
public:
    Shader(const std::string vertexShaderSource, const std::string fragmentShaderSource);
    GLuint getID();
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setDouble(const char* name, double value) const;
    void setBool(const char* name, bool value) const;

private:
    GLuint program;
    const char *vsSource, *fsSource;
    void addShader(const char* source, GLenum type);
    void checkProgramError() const;
};


#endif //GLPLAYGROUND_SHADER_H
