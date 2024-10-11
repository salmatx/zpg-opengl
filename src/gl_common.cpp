#include "gl_common.h"

#include <GL/glew.h>

#include <iostream>

namespace engine {
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLCallLog(const char* t_function, const char* t_file, int t_line)
{
    while (const GLenum error = glGetError())
    {
        std::cout << "OpenGL Error: " << "(" << error << ": " << GetGLErrorName(error) << ") " << t_function << " " << t_file << ":" << t_line << std::endl;
        return false;
    }
    return true;
}

std::string GetGLErrorName(unsigned int t_error_code) {
    switch (t_error_code) {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_CONTEXT_LOST:
        return "GL_CONTEXT_LOST";
    case GL_TABLE_TOO_LARGE:
        return "GL_TABLE_TOO_LARGE";
    default:
        return "UNKNOWN_ERROR";
    }
}
}
