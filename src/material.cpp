#include "material.hpp"
#include <memory>

Material::Material(const char *vertexPath, const char *fragmentPath,
                   std::optional<Texture> texture)
    : color(glm::vec4(1.0f)), map(texture),
      shader(std::make_unique<Shader>(vertexPath, fragmentPath)) {}

void Material::setColor(glm::vec4 newColor) { this->color = newColor; }

void Material::apply() {
  shader->use();
  shader->setVec4("color", this->color);
  if (map) {
    map->SetActive(this->shader.get(), 0);
  }
}

std::shared_ptr<Material> Material::clone() const {
    // Create a new material using private constructor
    auto cloned = std::shared_ptr<Material>(new Material());

    // Share the shader (using shared_ptr internally)
    cloned->shader = std::make_unique<Shader>(*shader);

    // Copy the color
    cloned->color = color;

    // Handle texture
    if (map.has_value()) {
      // Create a new texture sharing the same GPU resources
      cloned->map = map.value().clone();
    }

    return cloned;
}
