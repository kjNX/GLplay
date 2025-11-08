#ifndef GLPLAY_CONSTANTS_HPP
#define GLPLAY_CONSTANTS_HPP

constexpr unsigned int SCREEN_WIDTH{1024u};
constexpr unsigned int SCREEN_HEIGHT{768u};

constexpr unsigned int CIRCLE_SEGMENTS{64}; // i like level 24
constexpr unsigned int CIRCLE_RADIUS{256};

const char* vertexShaderSrc{R"glsl(
    #version 330 core
    layout (location=0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos, 1.f);
    }
)glsl"
};

const char* fragmentShaderSource = R"glsl(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)glsl";

#endif //GLPLAY_CONSTANTS_HPP