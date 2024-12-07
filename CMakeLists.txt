cmake_minimum_required(VERSION 3.29)
project(zpg)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)
if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# SOIL Library Configuration
set(SOIL_INCLUDE_DIR "")
set(SOIL_LIBRARY "")

if(WIN32)
    message(STATUS "Configuring for Windows...")
    set(SOIL_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/external/SOIL/include")
    set(SOIL_LIBRARY "${PROJECT_SOURCE_DIR}/external/SOIL/lib/SOIL.lib")
elseif(APPLE)
    message(STATUS "Configuring for macOS...")
    set(SOIL_INCLUDE_DIR "/usr/local/include/SOIL")
    set(SOIL_LIBRARY "/usr/local/lib/libSOIL.so")
elseif(UNIX)
    message(STATUS "Configuring for Linux...")
    set(SOIL_INCLUDE_DIR "/usr/include/SOIL")
    set(SOIL_LIBRARY "/usr/lib64/libSOIL.so")
endif()

if(NOT EXISTS ${SOIL_INCLUDE_DIR} OR NOT EXISTS ${SOIL_LIBRARY})
    message(FATAL_ERROR "SOIL library not found. Please ensure SOIL is installed or provide the correct paths.")
endif()

set(ENGINE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/engine")
set(APPLICATION_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/application")

file(GLOB ENGINE_SOURCES "${ENGINE_SOURCE_DIR}/*.cpp")
file(GLOB APPLICATION_SOURCES "${APPLICATION_SOURCE_DIR}/*.cpp")

set(PCH_HEADER "${ENGINE_SOURCE_DIR}/epch.h")

add_library(engine STATIC ${ENGINE_SOURCES})

target_include_directories(engine PUBLIC
        "${ENGINE_SOURCE_DIR}/interface"
        "${ENGINE_SOURCE_DIR}"
        "${SOIL_INCLUDE_DIR}"
)
target_precompile_headers(engine PRIVATE ${PCH_HEADER})

target_link_libraries(engine PRIVATE
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
        ${SOIL_LIBRARY}
)

add_executable(zpg ${APPLICATION_SOURCES})

target_include_directories(zpg PRIVATE "${PROJECT_SOURCE_DIR}/res/models")

target_link_libraries(zpg PRIVATE engine)

add_compile_options(-Wall -Wextra -pedantic)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(engine PRIVATE ASSERTATION_ENABLED=1)
else()
    target_compile_definitions(engine PRIVATE ASSERTATION_ENABLED=0)
endif()