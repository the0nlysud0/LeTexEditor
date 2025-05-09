cmake_minimum_required(VERSION 3.10)

project(LeTexEditor)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/Include)

# Create the executable
add_executable(LeTexEditor "Source/main.cpp"
    "Source/Shader.cpp"
    "Source/TextRenderer.cpp"
    "Source/ViewControl.cpp"
    "Source/InputSystem.cpp"
    )

add_definitions(-DGLEW_STATIC)
add_definitions(-DD_DEBUG)

# Link libraries (make sure the libraries are in the correct folder)
target_link_libraries(LeTexEditor 
    glfw3.lib
    glew32s.lib
    opengl32.lib
    freetype.lib
)
