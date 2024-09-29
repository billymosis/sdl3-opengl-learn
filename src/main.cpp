#include "SDL3/SDL_events.h"
#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>

#include "debug.hpp"
#include "gl_utils.hpp"
#include "mesh.hpp"
#include "window.hpp"

std::vector<VertexData> GenerateCube() {
  std::vector<VertexData> vertexData;

  //////// Generate vertex data
  VertexData data;

  // Front
  data.position = glm::vec3(0.0f, 0.5f, 0.0f);
  data.color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  vertexData.push_back(data);
  data.position = glm::vec3(0.5f, -0.5f, 0.0f);
  data.color = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
  vertexData.push_back(data);
  data.position = glm::vec3(-0.5f, -0.5f, 0.0f);
  data.color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
  vertexData.push_back(data);

  return vertexData;
}

int main(int argc, char *argv[]) {
  Window window(1280, 720, "Learn OpenGL Renderer");
  Debug debug(window.window, &window.gl_context);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  auto a = GenerateCube();

  std::vector<unsigned int> indices = {// Front face
                                       0, 1, 2, 1, 3, 2,
                                       // Right face
                                       4, 5, 6, 5, 7, 6,
                                       // Back face
                                       8, 9, 10, 9, 11, 10,
                                       // Left face
                                       12, 13, 14, 13, 15, 14,
                                       // Bottom face
                                       16, 17, 18, 17, 19, 18,
                                       // Top face
                                       20, 21, 22, 21, 23, 22};

  Mesh m(a);

  std::cout << m.GetVAO() << " " << m.GetVertexData().size() << std::endl;

  GLuint shader_prog = create_programme_from_files("../shaders/general.vert",
                                                   "../shaders/general.frag");
  glUseProgram(shader_prog);

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

    glBindVertexArray(m.GetVAO());
    glDrawArrays(GL_TRIANGLES, 0, 12);

    debug.Window1();

    debug.End();

    debug.Display();

    SDL_GL_SwapWindow(window.window);
  }

  debug.Destroy();
  window.Quit();
  return 0;
}
