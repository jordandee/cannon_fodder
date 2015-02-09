#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>
#include "resources.h"
#include "globals.h"

void loadSettings()
{
  std::ifstream ifs;
  ifs.open("settings.txt", std::ifstream::in);

  int n;
  std::string s;

  while (ifs >> s >> n)
  {
    if (s == "terrain_type")
      gTerrainType = n;
    if (s == "obstacles")
      gObstacleTotal = n;
    if (s == "wind")
      gWind = n;
  }
  // TODO: need bounds check
  ifs.close();
}

// Loads image to a texture
SDL_Texture* loadTexture(const char* filepath, SDL_Renderer* renderer)
{
  SDL_Texture* newTexture = NULL;

  SDL_Surface* loadedSurface = IMG_Load(filepath);
  if (loadedSurface == NULL)
  {
    std::cout << "IMG load error: " << IMG_GetError() << std::endl;
  }
  else
  {
    SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
    newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (newTexture == NULL)
    {
      std::cout << "Create texture error: " << SDL_GetError() << std::endl;
    }
    SDL_FreeSurface(loadedSurface);
  }
  return newTexture;
}
