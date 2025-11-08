#include <cstdio>
#include <exception>
#include <cmath>
#include <stdexcept>

#include "glad-compat/include/glad/glad.h"
#include "GLFW/glfw3.h"

#include "constants.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // double aspect{width / static_cast<double>(height)};
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    try {
        if(!glfwInit()) throw std::runtime_error("Failed to initialize GLFW");
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

        GLFWwindow* window{glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello World!!!", nullptr, nullptr)};
        if(!window) throw std::runtime_error("Failed to create GLFW window.");
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) throw std::runtime_error("Failed to initialize GLAD");

        while(!glfwWindowShouldClose(window)) {
            processInput(window);

            glClearColor(.2f, .3f, .3f, 1.f);
            glClear(GL_COLOR_BUFFER_BIT);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glBegin(GL_LINE_LOOP);

            float angleFragment{2 * M_PI / CIRCLE_SEGMENTS};
            float colorFragment{1 / static_cast<float>(CIRCLE_SEGMENTS)};
            for(int i{0}; i < CIRCLE_SEGMENTS; ++i) {
                float angle{i * angleFragment};
                float color{i * colorFragment};
                glColor3f(color, color, color);
                // glColor3f(1, 1, 1);
                glVertex3f(SCREEN_WIDTH/2.f + CIRCLE_RADIUS * cosf(angle), SCREEN_HEIGHT/2.f + CIRCLE_RADIUS * sinf(angle), 0);
            }

            // glColor3f(1.f, 0.f, 0.f);
            // glVertex3f(640, 0, 0);

            // glColor3f(0.f, 1.f, 0.f);
            // glVertex3f(520, 120, 0);

            // glColor3f(0.f, 0.f, 1.f);
            // glVertex3f(760, 120, 0);

            glEnd();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    } catch (std::exception& e) {
        printf("ERROR: %s\n", e.what());
    }
    glfwTerminate();
    return 0;
}