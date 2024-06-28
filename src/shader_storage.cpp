#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "shader_storage.h"

ShaderStorage::ShaderStorage()
{
    // 폴더가 없다면 생성
}

const std::string ShaderStorage::get(std::string file)
{
    std::string source;
    std::ifstream fs;

    fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        fs.open("shaders/" + file);

        std::stringstream shaderStream;

        shaderStream << fs.rdbuf();
        fs.close();

        source = shaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Failed to read file." << std::endl;
    }

    return source;
}