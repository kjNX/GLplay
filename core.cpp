#include <cstdio>
#include <exception>
#include <stdexcept>

#include "glad-core/include/glad/glad.h"
#include "GLFW/glfw3.h"

#include "constants.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool checkShaderCompilation(const unsigned int& shader) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", infoLog);
    }
    return success;
}

bool checkProgramLinking(const unsigned int& program) {
    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", infoLog);
    }
    return success;
}

int main() {
    try {
        if(!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window{glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World!!!", nullptr, nullptr)};
        if(!window) throw std::runtime_error("Failed to create GLFW window.");
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) throw std::runtime_error("Failed to initialize GLAD");

        unsigned int vertexShader{glCreateShader(GL_VERTEX_SHADER)};
        glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
        glCompileShader(vertexShader);
        checkShaderCompilation(vertexShader);

        unsigned int fragmentShader{glCreateShader(GL_FRAGMENT_SHADER)};
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);
        checkShaderCompilation(fragmentShader);

        unsigned int program{glCreateProgram()};
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        checkProgramLinking(program);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] {
            -.5f, -.5f, .0f,
            .5f, -.5f, .0f,
            .0f, .5f, .0f
        };

        unsigned int VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        while(!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(.2f, .3f, .3f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);
            glUseProgram(program);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(program);
    } catch (std::exception& e) {
        printf("ERROR: %s\n", e.what());
    }
    glfwTerminate();
    return 0;
}