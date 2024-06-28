#include <iostream>
#include "shader.h"

Shader::Shader(const std::string vertexShaderSource, const std::string fragmentShaderSource)
{
    vsSource = vertexShaderSource.c_str();
    fsSource = fragmentShaderSource.c_str();

    program = glCreateProgram();

    addShader(vertexShaderSource.c_str(), GL_VERTEX_SHADER);
    addShader(fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);

    glLinkProgram(program);
    checkProgramError();
}

GLuint Shader::getID()
{
    return program;
}

void Shader::setInt(const char *name, int value) const
{
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::setFloat(const char *name, float value) const
{
    glUniform1f(glGetUniformLocation(program, name), value);
}

void Shader::setDouble(const char *name, double value) const
{
    glUniform1d(glGetUniformLocation(program, name), value);
}

void Shader::setBool(const char *name, bool value) const
{
    glUniform1i(glGetUniformLocation(program, name), value);
}

void Shader::addShader(const char *source, GLenum type)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    GLint success;
    GLchar infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Failed to compile vertex shader." << std::endl << infoLog << std::endl;
    }

    glAttachShader(program, shader);
}

void Shader::checkProgramError() const
{
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        std::cout << "Failed to link shader program." << std::endl << infoLog << std::endl;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, sizeof(infoLog), NULL, infoLog);
        std::cout << "Failed to validate shader program." << std::endl << infoLog << std::endl;
    }
}