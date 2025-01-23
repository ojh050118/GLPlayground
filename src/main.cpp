#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include "shader_storage.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

const float toRadians = 3.14159265f / 180.0f;

ShaderStorage shaders = ShaderStorage();

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

    int bufferWidth, bufferHeight;
    glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glEnable(GL_DEPTH_TEST);

    // prepare to rendering
    GLfloat vertices[] = {
            // position             // color                // texture coordinate
            -0.5f, -0.5f, 0.5f,     1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
            0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,
            0.5f, 0.5f, 0.5f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f,      1.0f, 0.0f, 0.0f,       0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,       0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,      0.0f, 1.0f, 0.0f,       1.0f, 0.0f,
            0.5f, 0.5f, -0.5f,       0.0f, 0.0f, 1.0f,       1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f,      1.0f, 0.0f, 0.0f,       0.0f, 1.0f,
    };

    unsigned int indices[] = {
            0, 2, 3,
            0, 1, 2,
            1, 6, 2,
            1, 5, 6,
            5, 7, 6,
            5, 4, 7,
            4, 3, 7,
            4, 0, 3,
            0, 5, 4,
            0, 1, 5,
            3, 6, 7,
            3, 2, 6
    };

    GLuint VAO, VBO, EBO;
    GLint uniformModel, uniformProjection;

    GLuint texture[2];
    int imageWidth, imageHeight, nrChannels;

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

    // Texture Load
    glGenTextures(2, texture);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("textures/1.jpg", &imageWidth, &imageHeight, &nrChannels, 0);

    if (!data)
        std::cout << "Failed to load texture image." << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("textures/2.png", &imageWidth, &imageHeight, &nrChannels, 0);

    if (!data)
        std::cout << "Failed to load texture image." << std::endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (void*)(sizeof(GLfloat) * 6));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    Shader shader = Shader(shaders.get("vertexShader.vs"), shaders.get("fragmentShader.fs"));

    uniformModel = glGetUniformLocation(shader.getID(), "model");
    uniformProjection = glGetUniformLocation(shader.getID(), "projection");

    // aspect = width / height
    glm::mat4 projection = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

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

        rotation += 4.2f;

        if (rotation >= 360)
            rotation -= 360;

        if (scalingDirection)
            currentSize += 0.002f;
        else
            currentSize -= 0.002f;

        if (currentSize >= maxSize || currentSize <= minSize)
            scalingDirection = !scalingDirection;


        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0ff));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::rotate(model, rotation * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
//        model = glm::scale(model, glm::vec3(currentSize , currentSize, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f , 0.5f, 0.5f));

        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

        // Texture Unit
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[1]);
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

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
