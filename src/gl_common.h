#pragma once

#ifdef ASSERTATION_ENABLED
#else
#define ASSERTATION_ENABLED 0
#endif

#if ASSERTATION_ENABLED == 1

#define debugBreak() __builtin_trap()

#define ASSERT(expr) \
    if (expr) { } \
    else { \
        debugBreak(); \
    } \

#define GLCall(expr) engine::GLClearError(); \
    expr; \
    ASSERT(engine::GLCallLog(#expr, __FILE__, __LINE__))

#else

#define ASSERT(expr)
#define GLCall(expr) expr

#endif

#include <string>

namespace engine {

void GLClearError();
bool GLCallLog(const char* t_function, const char* t_file, int t_line);
std::string GetGLErrorName(unsigned int t_error_code);

}

