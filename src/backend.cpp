#include "backend.h"
#include "opengl.h"
#include <iostream>
#include <string>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

namespace Backend {
    GLFWwindow* _window;
    int _width = 800, _height = 700;
    bool _forceCloseWindow = false;

    void init() {
        glfwInit();
        glfwSetErrorCallback([](int error, const char* description) { std::cout << "GLFW Error (" << std::to_string(error) << "): " << description << "\n"; });
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

        _window = glfwCreateWindow(_width, _height, "Unloved", nullptr, nullptr);
        glfwMakeContextCurrent(_window);
        glfwSetFramebufferSizeCallback(_window, framebufferSizeCallback);

        OpenGL::init();
    }

    int getWinWidth() {
        return _width;
    }

    int getWinHeight() {
        return _height;
    }

    bool windowIsOpen() {
        return !glfwWindowShouldClose(_window) && !_forceCloseWindow;
    }

    GLFWwindow* getWindowPointer() {
        return _window;
    }

    void forceCloseWindow() {
        _forceCloseWindow = true;
    }

    void beginFrame() {
        glfwPollEvents();
        glfwSwapBuffers(_window);
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Backend::_width = width; Backend::_height = height;
}