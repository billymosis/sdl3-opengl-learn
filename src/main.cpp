#include "SDL3/SDL_events.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <memory>

#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_scancode.h"
#include "SDL3/SDL_stdinc.h"
#include "camera.hpp"
#include "debug.hpp"
#include "helper.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "window.hpp"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int forward = 0; // Forward/backward movement
int right = 0;   // Left/right movement

bool firstMouse = false;
bool enableMouse = true;

void mouse_callback(Camera &camera, float xposIn, float yposIn, float lastX,
                    float lastY) {
  if (enableMouse) {
    camera.ProcessMouseMovement(xposIn, -yposIn);
  }
}

int main(int argc, char *argv[]) {

  Window window(1280, 720, "Learn OpenGL Renderer");
  Debug debug(window.window, &window.gl_context);

  double lastX = window.width / 2.0f;
  double lastY = window.height / 2.0f;

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  const std::shared_ptr<Mesh> m = std::make_shared<Mesh>();
  GenerateCube(m);

  Shader cube("../shaders/general.vert", "../shaders/general.frag");
  cube.use();

  Uint32 previousTicks = SDL_GetTicks();

  float mouseX, mouseY;
  // Main loop
  bool done = false;
  int direction{0};
  while (!done) {
    Uint32 currentTicks = SDL_GetTicks();
    float deltaTime =
        (currentTicks - previousTicks) / 1000.0f; // Convert to seconds
    previousTicks = currentTicks;                 // Update the previous ticks
    auto *keystate = SDL_GetKeyboardState(NULL);

    Uint32 buttons = SDL_GetRelativeMouseState(&mouseX, &mouseY);
    mouse_callback(camera, mouseX, mouseY, lastX, lastY);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      debug.Event(&event);
      std::cout << event.key.scancode << std::endl;
      switch (event.type) {
      case SDL_EVENT_QUIT:
        done = true;
        break;
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
        SDL_PumpEvents();
        if (event.button.button == SDL_BUTTON_LEFT) {
          enableMouse = !enableMouse;
         SDL_SetWindowRelativeMouseMode(window.window, enableMouse);
        }
        break;
      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP: {
        bool keyPressed = (event.type == SDL_EVENT_KEY_DOWN);
        switch (event.key.scancode) {
        case SDL_SCANCODE_ESCAPE:
          done = true;
          break;

        case SDL_SCANCODE_W:
          forward = keyPressed ? 1 : 0; // Move forward
          break;

        case SDL_SCANCODE_S:
          forward = keyPressed ? -1 : 0; // Move backward
          break;

        case SDL_SCANCODE_D:
          right = keyPressed ? 1 : 0; // Move right
          break;

        case SDL_SCANCODE_A:
          right = keyPressed ? -1 : 0; // Move left
          break;

        default:
          break;
        }
        break;
      }
      }
    }

    camera.CameraMove(forward, right, deltaTime);

    if (SDL_GetWindowFlags(window.window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    debug.Start();

    float width = debug.io->DisplaySize.x;
    float height = debug.io->DisplaySize.y;

    // Rendering
    glViewport(0, 0, (int)width, (int)height);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            width / height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    cube.setMat4("projection", projection);
    cube.setMat4("view", view);

    // Cube Transformation
    glm::mat4 model = glm::mat4(1.0f);
    cube.setMat4("model", model);

    glBindVertexArray(m->GetVAO());
    glDrawElements(GL_TRIANGLES, m->GetIndices().size(), GL_UNSIGNED_INT, 0);

    debug.Window1();

    debug.End();

    debug.Display();

    SDL_GL_SwapWindow(window.window);
  }

  debug.Destroy();
  window.Quit();
  return 0;
}
