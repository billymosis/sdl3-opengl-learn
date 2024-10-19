#include "app.hpp"
#include "cube.hpp"
#include "debug.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include "sphere.hpp"
#include "window.hpp"

#include "camera.hpp"
#include "entity.hpp"
#include "model.hpp"
#include "shader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

App::App() {
  this->camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  Window window(1280, 720, "Learn OpenGL Renderer");
  Debug debug(window.window, &window.gl_context);

  stbi_set_flip_vertically_on_load(true);

  glEnable(GL_DEPTH_TEST);

  // Our state
  Shader ourShader("../shaders/model_loading.vert",
                   "../shaders/model_loading.frag");
  ourShader.use();

  Cube c;
  Cube s(2.0f, 0.5f);
  Model m(c);
  Model m2(s);
  Entity wak(m);

  const float scalex = 10.0f;
  wak.transform.setLocalScale({scalex, scalex, scalex});
  wak.addChild(m);
  wak.addChild(m2);
  wak.updateSelfAndChild();

  // load entities
  // -----------
  Model model = Model("../assets/planet.obj");
  Entity ourEntity(model);
  ourEntity.transform.setLocalPosition({10, 0, 0});
  const float scale = 0.75;
  ourEntity.transform.setLocalScale({scale, scale, scale});

  {
    Entity *lastEntity = &ourEntity;

    for (unsigned int i = 0; i < 10; ++i) {
      lastEntity->addChild(model);
      lastEntity = lastEntity->children.back().get();

      // Set transform values
      lastEntity->transform.setLocalPosition({10, 0, 0});
      lastEntity->transform.setLocalScale({scale, scale, scale});
    }
  }

  ourEntity.updateSelfAndChild();

  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  Uint32 previousTicks = SDL_GetTicks();

  // Main loop
  bool done = false;
  while (!done) {
    Uint32 currentTicks = SDL_GetTicks();
    float deltaTime =
        (currentTicks - previousTicks) / 1000.0f; // Convert to seconds
    previousTicks = currentTicks;                 // Update the previous ticks
    auto *keystate = SDL_GetKeyboardState(NULL);

    Uint32 buttons = SDL_GetRelativeMouseState(&mouseX, &mouseY);
    this->MouseCallback();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      debug.Event(&event);
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

    camera->CameraMove(forward, right, deltaTime);

    if (SDL_GetWindowFlags(window.window) & SDL_WINDOW_MINIMIZED) {
      SDL_Delay(10);
      continue;
    }

    debug.Start();

    float width = debug.io->DisplaySize.x;
    float height = debug.io->DisplaySize.y;

    // Rendering
    glViewport(0, 0, (int)width, (int)height);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(
        glm::radians(camera->Zoom), (float)window.width / (float)window.height,
        0.1f, 100.0f);
    glm::mat4 view = camera->GetViewMatrix();
    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);

    // draw our scene graph
    Entity *lastEntity = &ourEntity;
    while (lastEntity->children.size()) {
      ourShader.setMat4("model", lastEntity->transform.getModelMatrix());
      lastEntity->pModel->Draw(ourShader);
      lastEntity = lastEntity->children.back().get();
    }

    ourEntity.transform.setLocalRotation(
        {0.f, ourEntity.transform.getLocalRotation().y + 20 * deltaTime, 0.f});
    ourEntity.updateSelfAndChild();

    Entity *l = &wak;
    while (l->children.size()) {
      ourShader.setMat4("model", l->transform.getModelMatrix());
      l->pModel->Draw(ourShader);
      l = l->children.back().get();
    }

    wak.updateSelfAndChild();

    debug.Window1();

    debug.End();

    debug.Display();

    SDL_GL_SwapWindow(window.window);
  }

  debug.Destroy();
  window.Quit();
}

App::~App() {}

void App::MouseCallback() {
  if (this->enableMouse) {
    this->camera->ProcessMouseMovement(mouseX, -mouseY);
  }
}
