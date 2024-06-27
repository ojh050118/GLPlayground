#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
                          "layout (location = 0) in vec3 vPos;\n"
                          "layout (location = 1) in vec3 vCol;\n"
                          "\n"
                          "out vec3 vColor;\n"
                          "\n"
                          "void main()\n"
                          "{\n"
                          "    gl_Position = vec4(0.8f * vPos.x, 0.8f * vPos.y, 0.8f * vPos.z, 1.0);\n"
                          "    vColor = vCol;\n"
                          "}";

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

const char* fragmentShaderSource2 = "#version 330 core\n"
                                    "in vec3 vColor;\n"
                                    "out vec4 FragColor;\n"
                                    "\n"
                                    "uniform float time;\n"
                                    "\n"
                                    "void main()\n"
                                    "{\n"
                                    "    FragColor = vec4(vColor.x * abs(tan(time)), vColor.y * abs(tan(time * 2)), vColor.z * abs(tan(time * 3)), 1.0f);\n"
                                    "} ";

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

    // prepare to rendering
    GLfloat vertices[] = {
            -0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
            -1.0f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    GLfloat vertices2[] = {
            1.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint VAO[2], VBO[2], shaderProgram, shaderProgram2;

    glGenVertexArrays(2, VAO);
    glBindVertexArray(VAO[0]);

    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (void*)(sizeof(GLfloat) * 3));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    GLint success;
    GLchar infoLog[1024];

    shaderProgram = glCreateProgram();

    addShader(shaderProgram, vertexShaderSource, GL_VERTEX_SHADER);
    addShader(shaderProgram, fragmentShaderSource, GL_FRAGMENT_SHADER);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "Failed to link shader program." << std::endl << infoLog << std::endl;
    }

    glValidateProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "Failed to validate shader program." << std::endl << infoLog << std::endl;
    }

    shaderProgram2 = glCreateProgram();

    addShader(shaderProgram2, vertexShaderSource, GL_VERTEX_SHADER);
    addShader(shaderProgram2, fragmentShaderSource2, GL_FRAGMENT_SHADER);

    glLinkProgram(shaderProgram2);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform1f(glGetUniformLocation(shaderProgram, "time"), (float)glfwGetTime());

        glUseProgram(shaderProgram2);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUniform1f(glGetUniformLocation(shaderProgram2, "time"), (float)glfwGetTime());

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
