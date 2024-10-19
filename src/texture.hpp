#pragma once

#include "shader.hpp"
#include <string>
#include <glad/glad.h>

class Texture {
public:
  Texture();
  ~Texture();

  bool Load(const std::string &fileName);
  void Unload();
  void CreateFromSurface(struct SDL_Surface *surface);
  void CreateForRendering(int width, int height, unsigned int format);

  void SetActive(Shader &shader, int index = 0);

  int GetWidth() const { return mWidth; }
  int GetHeight() const { return mHeight; }
  unsigned int GetTextureID() const { return mTextureID; }

  const std::string &GetFileName() const { return mFileName; }

private:
  std::string mFileName;
  unsigned int mTextureID;
  int mWidth;
  int mHeight;
};
