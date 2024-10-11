cmake_minimum_required(VERSION 3.29)
project(zpg)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

option(DISABLE_ASAN "Do not use Address sanitizer" OFF)

if(NOT DISABLE_ASAN)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

add_compile_options(-Wall -Wextra -pedantic)

set (source_dir "${PROJECT_SOURCE_DIR}/src")

file(GLOB source_files "${source_dir}/*.cpp")

add_executable(zpg ${source_files})

target_link_libraries(zpg OpenGL::GL)
target_link_libraries(zpg GLEW::GLEW)
target_link_libraries(zpg glfw)
target_link_libraries(zpg glm::glm)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(zpg PRIVATE ASSERTATION_ENABLED=1)
else()
    target_compile_definitions(zpg PRIVATE ASSERTATION_ENABLED=0)
endif()