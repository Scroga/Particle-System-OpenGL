# Particle System
## Author: Ilia Riabko

This project implements a real-time particle system in OpenGL. It demonstrates three particle effects: snow, sparks, and fire. Each particle has its own position, velocity, acceleration, lifetime, color, size, and texture index. The particle state is updated every frame on the CPU.

The system is based on a common `ParticleGeneratorBase` class, which handles particle storage, spawning, buffer setup, and uploading instance data to the GPU. Specific effects are implemented in derived classes, where each class defines its own particle movement and appearance rules.

Particles are rendered using instanced `GL_POINTS`. A geometry shader expands each point into a camera-facing quad, so each particle behaves like a billboard. Textures are stored in a `sampler2DArray` uniform texture array, allowing every particle to select a different texture layer using its texture index.

## Controls
| Control          | Action                             |
| ---------------- | ---------------------------------- |
| **W / S**        | Move camera forward / backward     |
| **A / D**        | Move camera left / right           |
| **Q / E**        | Move camera down / up              |
| **Mouse**        | Rotate camera                      |
| **R**            | Reset camera position and rotation |
| **I**            | Print controls information         |
| **1**            | Switch to the snow scene           |
| **2**            | Switch to the sparks scene         |
| **3**            | Switch to the fire scene           |


## Dependencies
### CMake
Folders such as `resources/` and `shaders/` are automatically copied into appropriate directory on each application build, so there is no need to copy them manually.

### Loading library

To use modern OpenGL you need [loading library](https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library), which will set the function pointers for the API calls and load vendor extensions.

There is several options for such libraries. I chose [GLAD](https://github.com/Dav1dde/glad) as it should be sufficient
for our purposes. I generated basic version of the lib for the OpenGL 4.4 and it is stored in this repository in the `glad` directory, so there is no need for additional steps.


#### Why simple GL.h include does not suffice

Limited to OpenGL's Core Version: GL.h typically corresponds to OpenGL version 1.1, which is severely outdated. 
It doesn't include function pointers for modern OpenGL features or extensions.

Platform-Specific Function Pointers: OpenGL's design requires that function pointers for newer features be obtained at runtime. 
This is because different graphics drivers may implement different versions of OpenGL and support different sets of extensions. 
A static header file like GL.h cannot provide this level of flexibility.

## Windowing library

OpenGL requires either native window or window created by specialized library, which will provide OpenGL context and framebuffer.
Basic multiplatform solution is [GLFW](https://www.glfw.org/).

## GLM

[GLM](https://github.com/g-truc/glm) provides linear algebra library with similar syntax to GLSL. Download [here](https://github.com/g-truc/glm/releases/tag/1.0.1).