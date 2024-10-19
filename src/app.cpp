#include "app.hpp"
#include "SDL3/SDL_log.h"
#include "cube.hpp"
#include "debug.hpp"
#include "glm/fwd.hpp"
#include "node.hpp"
#include "shader.hpp"
#include "sphere.hpp"
#include "window.hpp"

#include "camera.hpp"
#include "model.hpp"
#include "shader.hpp"
#include <memory>

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

  auto t1 = std::make_shared<Texture>();
  t1->Load("../assets/awesomeface.png");
  auto t2 = std::make_shared<Texture>();
  t2->Load("../assets/container2.png");

  std::cout << "1: " << t1->GetTextureID() << "\n";
  std::cout << "2: " << t2->GetTextureID() << "\n";
  Cube c;
  Cube b;
  Sphere s;

  auto root = std::make_shared<Node>(c);
  auto ball = std::make_shared<Node>(s);
  auto c2 = std::make_shared<Node>(b,t1);
  root.get()->add(ball);
  ball->setPos(glm::vec3(2.0f, 2.0f, 2.0f));
  ball->add(c2);
  c2->setPos(glm::vec3(1.0f, 0.0f, 1.0f));
  root.get()->updateWorldTransform();
  Uint32 previousTicks = SDL_GetTicks();

  float rotation = 0.0;
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
    rotation += glm::radians(180.0f) * deltaTime;
    root.get()->setRot(glm::vec3(0, rotation, 0));
    root.get()->updateWorldTransform();
    root.get()->draw(ourShader);

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
