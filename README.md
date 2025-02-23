# zpg-opengl

- Simple opengl wrapper for creating scenes.
- Objects can be static or dynamic.
- Prepared shader for Skybox and Phong light model.

## Dependencies

1. OpenGL
2. GLEW
3. GLFW
4. Assimp
5. SOIL

## Build

### Linux (Ubuntu/Debian)

```shell
sudo apt update
sudo apt install cmake g++ libglew-dev libglfw3-dev libglm-dev libassimp-dev
sudo apt install libsoil-dev
```

### Linux (Fedora)

```shell
sudo dnf upgrade
sudo dnf install cmake gcc-c++ glew-devel glfw-devel glm-devel assimp-devel
sudo dnf install soil-devel
```

### Windows

Easiest way is by using package manager, e.g. Vcpkg or Conan.
```shell
vcpkg install glew glfw3 glm assimp soil:x64-windows
```
Ensure CMake is installed and added to PATH.


### Preview images
<img src="https://github.com/salmatx/MovieLibrary/blob/master/preview/scene_example.png" width="500" />