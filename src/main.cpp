#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "shader_storage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

const char* fragmentShaderSource = "#version 330 core\n"
                                   "in vec3 vColor;\n"
                                   "out vec4 FragColor;\n"
                                   "\n"
                                   "uniform float time;\n"
                                   "\n"
                                   "void main()\n"
                                   "{\n"
                                   "    FragColor = vec4(vColor, 1.0f);\n"
                                   "} ";

const float toRadians = 3.14159265f / 180.0f;

ShaderStorage shaders = ShaderStorage();

void addShader(GLuint program, const char* source, GLenum type)
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

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Playground", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();

        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    // prepare to rendering
    GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f
    };

    unsigned int indices[] = {
            0, 3, 1,
            1, 3, 2,
            2, 3, 0,
            0, 1, 2
    };

    GLuint VAO, VBO, EBO;
    GLint uniformModel;

    float triOffset = 0;
    float triIncrement = 0.01f;
    float maxTriOffset = 0.5f;
    bool direction;

    bool scalingDirection = true;
    float currentSize = 1.0f;
    float minSize = 0.1f;
    float maxSize = 2.0f;

    float rotation = 0;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    Shader shader = Shader(shaders.get("vertexShader.vs"), shaders.get("fragmentShader.fs"));

    uniformModel = glGetUniformLocation(shader.getID(), "model");

    // Should unbind the EBO AFTER unbind the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader.getID());

        if (direction)
            triOffset += triIncrement;
        else
            triOffset -= triIncrement;

        if (abs(triOffset) > maxTriOffset)
            direction = !direction;

        rotation += 0.1f;

        if (rotation >= 360)
            rotation -= 360;

        if (scalingDirection)
            currentSize += 0.002f;
        else
            currentSize -= 0.002f;

        if (currentSize >= maxSize || currentSize <= minSize)
            scalingDirection = !scalingDirection;


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, rotation * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
//        model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(currentSize , currentSize, 1.0f));
//
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        shader.setFloat("time", (float)glfwGetTime());
        //shader.setFloat("model", (float)glfwGetTime());

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
