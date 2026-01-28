# OpenGL Starter Template

A minimal OpenGL starter template for graphics programming labs and projects. Features a simple colored triangle demo with camera controls, ImGui integration, and a clean project structure ready for extension.

## Features

- **Modern OpenGL 3.3 Core Profile**
- **GLFW** for window management and input
- **GLM** for mathematics
- **GLAD** for OpenGL function loading
- **ImGui** for UI controls
- **Camera System** with FPS-style controls
- **Shader Loading** utility
- **Model Loading** support (via Assimp)
- Ready-to-use build scripts

## Prerequisites

### macOS
Install dependencies using Homebrew:

> **Note:** macOS Tahoe and above do not support CMake 3 via Homebrew. You must download and install CMake 3.x directly from the [official CMake website](https://cmake.org/download/) if you're on macOS Tahoe or later.

```bash
brew install cmake glfw glew assimp glm
```

### Linux
```bash
sudo apt-get install cmake libglfw3-dev libglew-dev libassimp-dev libglm-dev
```

### Windows
Use vcpkg or manually install:
- CMake
- GLFW
- GLEW
- Assimp
- GLM

## Project Structure

```
OpenGL_Starter_Template/
├── src/
│   └── main.cpp              # Main application
├── include/
│   ├── camera.h              # Camera system
│   ├── model.h               # Model loading utilities
│   └── shaders.h             # Shader loading utilities
├── assets/
│   ├── shaders/
│   │   ├── main.vert         # Vertex shader
│   │   └── main.frag         # Fragment shader
│   └── models/               # Place your 3D models here
├── external/
│   ├── glad/                 # OpenGL loader
│   └── imgui/                # ImGui library
├── build.sh                  # Build script
├── run.sh                    # Run script
└── CMakeLists.txt           # CMake configuration
```

## Getting Started

### Option 1: Copy Template for New Project (Recommended)

To use this as a starting point for a new project:

```bash
# Copy the template to your new project directory
cp -r OpenGL_Starter_Template YourProjectName
cd YourProjectName

# Remove git history and start fresh
rm -rf .git
git init
git add .
git commit -m "Initial commit from OpenGL starter template"

# Optional: Add your remote repository
git remote add origin <your-repo-url>
```

### Option 2: Clone Repository

```bash
git clone <your-repo-url>
cd OpenGL_Starter_Template
```

### 2. Build the Project

Make the build script executable and run it:

```bash
chmod +x build.sh
./build.sh
```

This will:
- Clean any existing build directory
- Create a fresh build directory
- Run CMake configuration
- Compile the project

### 3. Run the Application

```bash
chmod +x run.sh
./run.sh
```

Or run directly:

```bash
./build/myproject
```

## Controls

### UI Mode (Default)
- **TAB** - Toggle between UI mode and camera mode
- **ESC** - Close the application
- Mouse is visible and can interact with ImGui

### Camera Mode (Press TAB)
- **W/A/S/D** - Move forward/left/backward/right
- **SPACE** - Move up
- **LEFT SHIFT** - Move down
- **Mouse** - Look around
- **Scroll** - Zoom in/out

## Customization

### Adding Your Own Geometry

Replace the triangle setup in [src/main.cpp](src/main.cpp) (lines ~193-221) with your own vertex data, or use the Model loader:

```cpp
// Uncomment and modify:
Model myModel("assets/models/your_model.obj");

// In render loop:
myModel.Draw(shader);
```

### Modifying Shaders

Edit the shader files in `assets/shaders/`:
- [main.vert](assets/shaders/main.vert) - Vertex shader
- [main.frag](assets/shaders/main.frag) - Fragment shader

Changes will be reflected when you rebuild and run the project.

### Adding New Shaders

1. Create new `.vert` and `.frag` files in `assets/shaders/`
2. Load them in main.cpp:

```cpp
Shader myShader("assets/shaders/my_shader.vert", 
                "assets/shaders/my_shader.frag");
```

### Window Configuration

Modify window size and title in [src/main.cpp](src/main.cpp):

```cpp
int width = 1920, height = 1080;  // Line 21
glfwCreateWindow(width, height, "Your Title", nullptr, nullptr);  // Line 137
```

## Troubleshooting

### Build Errors

**CMake can't find packages:**
- Ensure all dependencies are installed via Homebrew (macOS) or apt (Linux)
- For macOS, verify Homebrew installation path: `/opt/homebrew`
- For custom install paths, modify `CMakeLists.txt`

**GLAD errors:**
- GLAD is included in `external/glad/` - no installation needed

### Runtime Errors

**"Failed to initialize GLAD":**
- OpenGL drivers may be outdated
- Ensure your GPU supports OpenGL 3.3+

**Shaders not loading:**
- Run the executable from the project root directory
- Ensure `assets/shaders/` path is correct relative to executable

**Black screen:**
- Check that shaders compiled successfully (check console output)
- Verify vertex data is correctly set up

## Next Steps

This template provides a foundation for:
- **Lighting systems** (Phong, PBR, etc.)
- **Texture mapping**
- **Advanced rendering** (deferred shading, shadows, etc.)
- **Post-processing effects**
- **Physics simulations**
- **Complex scene management**

Extend the code in `src/main.cpp` and add your own features as needed for your labs!

## License

MIT License - Feel free to use for educational purposes.

## Resources

- [LearnOpenGL](https://learnopengl.com/) - Excellent OpenGL tutorials
- [OpenGL Docs](https://www.khronos.org/opengl/) - Official documentation
- [GLM Documentation](https://glm.g-truc.net/) - Math library docs
- [ImGui](https://github.com/ocornut/imgui) - UI library