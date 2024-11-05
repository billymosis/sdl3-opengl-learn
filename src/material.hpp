#pragma once

#include "glm/glm.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include <memory>
#include <optional>

class Material {
public:
  Material(const char *vertexPath, const char *fragmentPath,
           std::optional<Texture> texture = std::nullopt);
  void apply(glm::mat4 worldMatrix);

  // Copy constructor - deleted to prevent accidental copies
  Material(const Material &) = delete;

  // Move constructor
  Material(Material &&other) noexcept
      : color(other.color), map(std::move(other.map)),
        shader(std::move(other.shader)) {}

  // Copy assignment - deleted to prevent accidental copies
  Material &operator=(const Material &) = delete;

  // Move assignment
  Material &operator=(Material &&other) noexcept {
    if (this != &other) {
      map = std::move(other.map);
      shader = std::move(other.shader);
      color = other.color;
    }
    return *this;
  }

  // Clone method that creates a new material sharing shader resources
  std::shared_ptr<Material> clone() const;

  ~Material() = default;

  const Shader &getShader() const { return *shader; }
  void setColor(glm::vec4 newColor);

private:
  Material() = default;
  glm::vec4 color{glm::vec4(1.0f)};
  std::optional<Texture> map;
  std::unique_ptr<Shader> shader;
};
