#include "glm/fwd.hpp"
#include "shader.hpp"

class Renderer {
  Shader shader;
  glm::mat4 modelMatrix;
  Renderer();
  ~Renderer();
};
