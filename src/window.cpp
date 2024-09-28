#include "window.hpp"
#include "SDL3/SDL_init.h"

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

  this->window = SDL_CreateWindow("Learn OpenGL Renderer", width, height,
                                  this->window_flags);

  if (window == nullptr) {
    throw std::runtime_error("Error: SDL_CreateWindow(): %s\n" +
                             std::string(SDL_GetError()));
  }

  SDL_SetWindowPosition(this->window, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);

  this->gl_context = this->gl_context = SDL_GL_CreateContext(this->window);

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1); // Enable vsync
  SDL_ShowWindow(window);
}

void Window::Quit() {
  SDL_GL_DestroyContext(this->gl_context);
  SDL_DestroyWindow(this->window);
  SDL_Quit();
}
