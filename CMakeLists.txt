cmake_minimum_required(VERSION 3.5)
project(learn_gl)

# Include openGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Support clangd lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL3
set(SDL_OPENGLES OFF)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)

# Include GLM
set(GLM_TEST_ENABLE OFF CACHE BOOL "GLM no test")
add_subdirectory(external/glm)

# Add GLAD
include_directories("${PROJECT_SOURCE_DIR}/external/glad/include")
set(GLAD_SOURCES "${PROJECT_SOURCE_DIR}/external/glad/src/glad.c")

# Include imgui 
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Source files this is bad, but this is small project
file(GLOB PROJECT_SOURCES src/*.cpp src/*.hpp)

# Create the executable
add_executable(learn_gl ${PROJECT_SOURCES} ${GLAD_SOURCES} ${IMGUI_SOURCES})

# Link SDL3 to the executable
target_link_libraries(learn_gl PRIVATE ${OPENGL_LIBRARIES} glm SDL3::SDL3)
