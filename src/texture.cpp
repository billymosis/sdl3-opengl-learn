#include "texture.hpp"
#include "SDL3/SDL_log.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <SDL3/SDL.h>

Texture::Texture() : mTextureID(0), mWidth(0), mHeight(0) {}

Texture::~Texture() {}

bool Texture::Load(const std::string &fileName) {
  mFileName = fileName;
  int channels = 0;

  // unsigned char *image = SOIL_load_image(fileName.c_str(), &mWidth, &mHeight,
  //                                        &channels, SOIL_LOAD_AUTO);
  unsigned char *image =
      stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 0);

  if (image == nullptr) {
    SDL_Log("STB failed to load image %s: %s", fileName.c_str());
    return false;
  }

  int format = GL_RGB;
  if (channels == 4) {
    format = GL_RGBA;
  }

  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);

  glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format,
               GL_UNSIGNED_BYTE, image);

  stbi_image_free(image);

  // Generate mipmaps for texture
  glGenerateMipmap(GL_TEXTURE_2D);
  // Enable linear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // // Enable anisotropic filtering, if supported
  // if (GLEW_EXT_texture_filter_anisotropic) {
  //   // Get the maximum anisotropy value
  //   GLfloat largest;
  //   glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largest);
  //   // Enable it
  //   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largest);
  // }

  return true;
}

void Texture::Unload() { glDeleteTextures(1, &mTextureID); }

void Texture::CreateFromSurface(SDL_Surface *surface) {
  mWidth = surface->w;
  mHeight = surface->h;

  // Generate a GL texture
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, surface->pixels);

  // Use linear filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::CreateForRendering(int width, int height, unsigned int format) {
  mWidth = width;
  mHeight = height;
  // Create the texture id
  glGenTextures(1, &mTextureID);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
  // Set the image width/height with null initial data
  glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, GL_RGB, GL_FLOAT,
               nullptr);

  // For a texture we'll render to, just use nearest neighbor
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Texture::SetActive(Shader &shader, int index) {
  glActiveTexture(GL_TEXTURE0 + index);
  glUniform1i(glGetUniformLocation(shader.ID, "texture_diffuse1"), index);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
}

void Texture::SetActive(Shader *shader, int index) {
  glActiveTexture(GL_TEXTURE0 + index);
  glUniform1i(glGetUniformLocation(shader->ID, "texture_diffuse1"), index);
  glBindTexture(GL_TEXTURE_2D, mTextureID);
}

Texture Texture::clone() const {
  Texture newTexture;
  // Share the GPU texture handle/ID
  newTexture.mTextureID = this->mTextureID;
  // Copy other necessary properties
  return newTexture;
}
