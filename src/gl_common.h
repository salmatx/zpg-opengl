#pragma once

#define ASSERTATION_ENABLED 1
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

namespace engine {

void GLClearError();
bool GLCallLog(const char* function, const char* file, int line);

}

