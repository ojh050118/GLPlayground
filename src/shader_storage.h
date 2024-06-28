#ifndef GLPLAYGROUND_SHADER_STORAGE_H
#define GLPLAYGROUND_SHADER_STORAGE_H


class ShaderStorage
{
public:
    ShaderStorage();
    const char* SHADER_DIRECTORY = "./Shaders";
    const std::string get(std::string file);
};


#endif //GLPLAYGROUND_SHADER_STORAGE_H
