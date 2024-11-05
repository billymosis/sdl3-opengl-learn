#include "app.hpp"
#include "cube.hpp"
#include "debug.hpp"
#include "glm/fwd.hpp"
#include "material.hpp"
#include "node.hpp"
#include "perspective_camera.hpp"
#include "shader.hpp"
#include "sphere.hpp"
#include "window.hpp"

#include "model.hpp"
#include "shader.hpp"
#include <memory>

App::App() {
  Window window(1280, 720, "Learn OpenGL Renderer");
  Debug debug(window.window, &window.gl_context);

  stbi_set_flip_vertically_on_load(true);

  glEnable(GL_DEPTH_TEST);

  auto t1 = std::make_shared<Texture>();
  t1->Load("../assets/awesomeface.png");
  auto t2 = Texture();
  t2.Load("../assets/container2.png");
  auto baseMaterial = std::make_shared<Material>(
      "../shaders/model_loading.vert", "../shaders/model_loading.frag", t2);

  // Clone materials with different colors
  auto materialRed = baseMaterial->clone();
  materialRed->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  auto materialGreen = baseMaterial->clone();
  materialGreen->setColor(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
  auto materialBlue = baseMaterial->clone();
  materialBlue->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

  auto cubeGeometry = std::make_shared<Cube>();
  auto sphereGeometry = std::make_shared<Sphere>();

  auto root = std::make_shared<Node>();

  auto cam = std::make_shared<PerspectiveCamera>(window);
  cam->setPos(glm::vec3(0.0f, 0.0f, 5.0f));
  root->add(cam);

  auto cube = std::make_shared<Node>(cubeGeometry, materialRed);
  root->add(cube);
  auto ball = std::make_shared<Node>(sphereGeometry, materialRed);
  ball->setPos(glm::vec3(2.0f, 2.0f, 2.0f));
  cube->add(ball);

  auto cube2 = std::make_shared<Node>(cubeGeometry, materialGreen);
  cube2->setPos(glm::vec3(2.0f, 2.0f, 0.0f));
  root->add(cube2);

  auto cube3 = std::make_shared<Node>(cubeGeometry, materialBlue);
  cube3->setPos(glm::vec3(5.0f, 5.0f, 0.0f));
  root->add(cube3);

  root->updateWorldTransform();

  Uint32 previousTicks = SDL_GetTicks();

  GLuint matricesUBO;
  // Create and bind UBO for matrices
  glGenBuffers(1, &matricesUBO);
  glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
  glBufferData(GL_UNIFORM_BUFFER, sizeof(GlobalMatrices), nullptr,
               GL_DYNAMIC_DRAW);
  // Bind to binding point 0
  glBindBufferBase(GL_UNIFORM_BUFFER, 0, matricesUBO);
  root->setUBOProgram(matricesUBO);

  float rotation = 0.0;
  // Main loop
  bool done = false;
  int frameCount = 0;
  while (!done) {
    Uint32 currentTicks = SDL_GetTicks();
    float deltaTime =
        (currentTicks - previousTicks) / 1000.0f; // Convert to seconds
    previousTicks = currentTicks;                 // Update the previous ticks
    auto *keystate = SDL_GetKeyboardState(NULL);

    Uint32 buttons = SDL_GetRelativeMouseState(&mouseX, &mouseY);
    cam->mouseMovement(mouseX, mouseY);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      debug.Event(&event);
      switch (event.type) {
      case SDL_EVENT_MOUSE_WHEEL: {
        cam->mouseScroll(event.wheel.y);
        break;
      }
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

    // Movement speed
    float speedMultiplier = 0.1f;
    glm::vec3 direction(0.0f);

    // Combine forward and right movement
    if (forward == 1) {
      direction += cam->getFront(); // Move forward
    } else if (forward == -1) {
      direction -= cam->getFront(); // Move backward
    }

    if (right == 1) {
      direction += cam->getRight(); // Move right
    } else if (right == -1) {
      direction -= cam->getRight(); // Move left
    }

    // Normalize the direction vector
    if (glm::length(direction) > 0) {
      direction = glm::normalize(direction);
    }

    // Apply movement
    cam->setPos(cam->getPos() + direction * speedMultiplier);

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

    rotation += glm::radians(180.0f) * deltaTime;
    cube->setRot(glm::vec3(0, rotation, 0));
    cube->setPos(glm::vec3(glm::sin(currentTicks * 0.001f) * 5.0f, 0.0f, 0.0f));

    root->updateWorldTransform();

    root->draw(cam.get());
    debug.Window1();

    debug.End();

    debug.Display();

    SDL_GL_SwapWindow(window.window);
  }

  debug.Destroy();
  window.Quit();
}

App::~App() {}
