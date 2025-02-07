#include "backend.h"
#include "opengl.h"
#include "input.h"


int main()
{
    Backend::init();
    Input::init();
    
    while (Backend::windowIsOpen()) {
        Backend::beginFrame();
        Input::update();
        OpenGL::render();
    }
}