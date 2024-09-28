#pragma once

#include "imgui.h"
#include <SDL3/SDL.h>

class Debug {
public:
  Debug(SDL_Window *window, SDL_GLContext *gl_context);
  ~Debug();

  ImGuiIO *io;

  void Window1();
  void Event(SDL_Event *event);
  void Destroy();
  void Start();
  void End();
  void Display();

private:
};
