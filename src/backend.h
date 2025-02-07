#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Backend {
    void init();
    GLFWwindow* getWindowPointer();
    int getWinWidth();
    int getWinHeight();
    bool windowIsOpen();
    void forceCloseWindow();
    void beginFrame();
}