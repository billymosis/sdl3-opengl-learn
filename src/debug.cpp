#include "debug.hpp"

#include <SDL3/SDL.h>
#include <imgui.h>

#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl3.h"

Debug::Debug(SDL_Window *window, SDL_GLContext *gl_context) {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForOpenGL(window, gl_context);
  ImGui_ImplOpenGL3_Init("#version 460");
  this->io = &io;
};

Debug::~Debug() {};

void Debug::Destroy() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();
}

void Debug::Start() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void Debug::End() { ImGui::Render(); };

void Debug::Window1() {
  ImGui::Begin("My Debug - Window 1");
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
              1000.0f / this->io->Framerate, this->io->Framerate);
  ImGui::End();
};

void Debug::Display() {
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
};

void Debug::Event(SDL_Event *event) { ImGui_ImplSDL3_ProcessEvent(event); };
