#pragma once
#include "glm/glm.hpp"

class App {
public:
  App();
  ~App();

private:
  int forward{0};
  int right{0};
  bool firstMouse{false};
  bool enableMouse{true};
  float mouseX{0}, mouseY{0};

  struct GlobalMatrices {
    glm::mat4 projection;
    glm::mat4 view;
  };
};
