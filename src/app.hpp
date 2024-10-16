#include "camera.hpp"

class App {
public:
  App();
  ~App();
  Camera *camera;

private:
  int forward{0};
  int right{0};
  bool firstMouse{false};
  bool enableMouse{true};
  void MouseCallback();
  float mouseX{0}, mouseY{0};
};
