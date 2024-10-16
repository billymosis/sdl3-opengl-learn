cmake_minimum_required(VERSION 3.5)
project(learn_gl)

# Disable compiler extensions, turn off the compability with c99 (polyfill)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set warning level for GCC/Clang
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set warning level for MSVC
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4)
endif()

# Set cpp versions
set (CMAKE_CXX_STANDARD 17)

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

# Include assimp
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Disable Assimp warnings as errors")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Assimp as a static library")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "Disable Assimp tools")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Disable Assimp tests")
add_subdirectory(external/assimp)

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
target_link_libraries(learn_gl PRIVATE SDL3::SDL3 glm assimp ${OPENGL_LIBRARIES})
