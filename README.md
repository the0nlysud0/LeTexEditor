# LeTexEditor 📝

LeTexEditor is a lightweight OpenGL-based text editor built with C++, GLFW, and FreeType. It supports text rendering with zooming and scrolling, and provides basic input handling.

## Features

- 🔠 Crisp font rendering using FreeType
- 🔍 Infinite zooming (via Ctrl + mouse scroll)
- 📜 Smooth vertical scrolling
- 📂 Load text files with `Ctrl + O`
- 💾 Remembers the last opened file between sessions

## Controls

| Key/Action        | Function                      |
|-------------------|-------------------------------|
| `Ctrl + O`        | Open a file                   |
| `Mouse Scroll`    | Scroll up/down                |
| `Ctrl + Scroll`   | Zoom in/out (infinitely)      |

## Dependencies

- [GLFW](https://www.glfw.org/)
- [GLEW](http://glew.sourceforge.net/)
- [FreeType](https://freetype.org/)
- [GLM](https://github.com/g-truc/glm)

## Build Instructions (CMake)

```bash
git [clone https://github.com/yourusername/LeTexEditor.git](https://github.com/the0nlysud0/LeTexEditor.git)
cd LeTexEditor
mkdir build && cd build
cmake ..
cmake --build .
./LeTexEditor
```
Make sure all dependencies are installed and available in your compiler/toolchain.

## File Structure
```bash
LeTexEditor/
├── Assets/
│   ├── Fonts/
│   └── Settings/lastfile.txt
├── src/
│   ├── main.cpp
│   ├── InputSystem.cpp
│   └── ...
├── include/
│   └── InputSystem.h
├── shaders/
└── CMakeLists.txt
```
Made with 💻 by [Mehdi B] :)
