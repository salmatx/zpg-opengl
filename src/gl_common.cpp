//
// Created by sardo on 10/9/24.
//

#include "gl_common.h"

#include <GL/glew.h>

#include <iostream>

namespace engine {

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCallLog(const char* function, const char* file, int line)
{
    while (const GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << error << "(" << function << ", " << line << ")" << std::endl;
        return false;
    }
    return true;
}
}
