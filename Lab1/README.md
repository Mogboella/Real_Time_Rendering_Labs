# Lab 1

### Lab 1: Shading Models Comparison

Lab 1 implements and compares three different shading models side-by-side:

- **Phong Shading** - Classic specular reflection model
- **Toon Shading** - Cel-shaded, cartoon-style rendering
- **Oren-Nayar Shading** - Physically-based diffuse reflection for rough surfaces

The lab features:

- Real-time rendering with OpenGL 3.3
- Interactive camera controls (WASD + mouse)
- ImGui-based UI for adjusting lighting and material parameters
- Model loading via Assimp
- Dynamic shader switching and parameter adjustment

## Prerequisites

### Required Dependencies

- **CMake** (version 3.10 or higher)
- **C++ Compiler** with C++11 support
- **OpenGL** 3.3 or higher
- **GLFW3** - Window and input handling
- **GLEW** - OpenGL extension loading
- **GLM** - OpenGL Mathematics library
- **Assimp** - Model loading
- **ZLIB** - Compression library
- **GLAD** - OpenGL function loader (has to be included in external/)
- **ImGui** - Immediate mode GUI (has to be included in external/)

### Platform Notes

The project is configured for macOS (using Homebrew paths), but can be adapted for Windows or Linux by modifying the CMakeLists.txt file.

## Building and Running

### macOS/Linux

1. Build the project:

```bash
chmod +x build.sh
./build.sh
```

1. Run the application:

```bash
chmod +x run.sh
./run.sh
```

### Manual Build

```bash
cd Lab1
mkdir -p build
cd build
cmake ..
make
./lab1
```

## Controls

### Camera Movement

- **W/S** - Move forward/backward
- **A/D** - Strafe left/right
- **Space** - Move up
- **Left Shift** - Move down
- **Mouse** - Look around (when UI is hidden)
- **Scroll** - Adjust zoom/FOV

### UI Controls

- **Tab** - Toggle UI visibility and enable/disable camera controls
- **ESC** - Exit application

### UI Parameters

The ImGui interface provides real-time control over:

**Lighting:**

- Light position (3D position controls)
- Light color (RGB picker)
- Light intensity (0.0 - 5.0)

**Materials:**

- Model colors (global or per-shader)
- Link/unlink individual model colors
- Oren-Nayar roughness parameter (0.0 - 1.0)

## Features

### Rendering Techniques

1. **Phong Shading**
   - Smooth specular highlights
   - Per-fragment lighting calculations
   - Classic ambient, diffuse, and specular components

2. **Toon Shading**
   - Discrete lighting levels
   - Outline rendering potential
   - Cartoon/cel-shaded aesthetic

3. **Oren-Nayar Reflectance**
   - Physically-based diffuse BRDF
   - Adjustable surface roughness
   - More realistic than Lambertian for rough surfaces

### Interactive Features

- Real-time parameter adjustment
- Side-by-side shader comparison
- Synchronized model rotation
- Dynamic lighting control
- Free-camera navigation

## Resources

Here are the tutorials and models I used for this project:

- [Oren-Nayer Tutorial](https://garykeen27.wixsite.com/portfolio/oren-nayar-shading)
- [Toon Shading Code Concept](https://github.com/gametutorials/tutorials/tree/master/OpenGL/Toon%20Shading)
- [Phong Shading Tutorial](https://learnopengl.com/Advanced-Lighting/Advanced-Lighting)
- [Toon Shading Tutorial](https://medium.com/geekculture/shader-journey-1-basic-toons-3dfc68b4139c)
- [Utah Teapot Model](https://graphics.cs.utah.edu/teapot/)
- [Kolobok Model](https://sketchfab.com/3d-models/kolobok-5a66128b7f344b02a27f257d4844b335)

---

**Note:** Make sure all dependencies are properly installed before building. On macOS, you can install most dependencies via Homebrew:

```bash
brew install cmake glfw glew glm assimp
```
