cmake_minimum_required(VERSION 3.29)
project(zpg)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set (source_dir "${PROJECT_SOURCE_DIR}/src/engine")

file(GLOB source_files "${source_dir}/*.cpp")

set(PCH_HEADER "${source_dir}/epch.h")
set(PCH_SOURCE "${source_dir}/epch.cpp")

add_library(pch STATIC ${PCH_SOURCE})

target_precompile_headers(pch PRIVATE ${PCH_HEADER})

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)

if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_compile_options(-Wall -Wextra -pedantic)

include_directories("${PROJECT_SOURCE_DIR}/res/models")

add_executable(zpg ${source_files})

target_link_libraries(zpg PRIVATE
        pch
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(zpg PRIVATE ASSERTATION_ENABLED=1)
else()
    target_compile_definitions(zpg PRIVATE ASSERTATION_ENABLED=0)
endif()