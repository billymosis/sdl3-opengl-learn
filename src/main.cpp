#include "SDL3/SDL_events.h"
#include "SDL3/SDL_opengl.h"
#include <SDL3/SDL.h>

#include "debug.hpp"
#include "window.hpp"

int main(int argc, char *argv[]) {

  Window window(1280, 720, "Learn OpenGL Renderer");
  Debug debug(window.window, &window.gl_context);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  // Main loop
  bool done = false;
  while (!done) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      debug.Event(&event);
      if (event.type == SDL_EVENT_KEY_DOWN) {
        if (event.key.key == SDLK_ESCAPE) {
          done = true;
        }
      }
      if (event.type == SDL_EVENT_QUIT)
        done = true;
      if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
          event.window.windowID == SDL_GetWindowID(window.window))
        done = true;
    }

    if (SDL_GetWindowFlags(window.window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    debug.Start();

    // Rendering
    glViewport(0, 0, (int)debug.io->DisplaySize.x,
               (int)debug.io->DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    debug.Window1();

    debug.End();

    debug.Display();

    SDL_GL_SwapWindow(window.window);
  }

  debug.Destroy();
  window.Quit();
  return 0;
}
