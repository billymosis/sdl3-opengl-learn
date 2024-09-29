#include "window.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include <glad/glad.h>

#include <iostream>
#include <ostream>

#include <stdexcept>

Window::Window(int width, int height, const char *title)
    : width(width), height(height) {

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    throw std::runtime_error("SDL_Init failed: " + std::string(SDL_GetError()));
  }

  // Create window with graphics context
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  this->window = SDL_CreateWindow("Learn OpenGL Renderer", width, height,
                                  this->window_flags);

  if (window == nullptr) {
    throw std::runtime_error("Error: SDL_CreateWindow(): %s\n" +
                             std::string(SDL_GetError()));
  }

  SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  this->gl_context = this->gl_context = SDL_GL_CreateContext(this->window);

  // GLAD load all OpenGL function pointer
  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_ShowWindow(window);
}

void Window::Quit() {
  SDL_GL_DestroyContext(this->gl_context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}
