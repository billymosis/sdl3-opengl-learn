#pragma once

#include "SDL3/SDL_video.h"

class Window {
public:
  int width = 1280;
  int height = 720;
  Window(int width, int height, const char *title);
  SDL_Window *window;
  SDL_GLContext gl_context;
  void Quit();

private:
  Uint32 window_flags =
      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN;
};
