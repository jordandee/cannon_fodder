#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>
#include <string>
#include "resources.h"
#include "globals.h"
#include "math.h"

void loadSettings()
{
  std::ifstream ifs;
  ifs.open("settings.txt", std::ifstream::in);

  int n;
  std::string s;

  while (ifs >> s >> n)
  {
    if (s == "terrain_type")
      gTerrainOption = n;
    if (s == "obstacles")
      gObstaclesOption = n;
    if (s == "wind")
      gWindOption = n;
    if (s == "fullscreen")
      gFullScreenOption = n;
  }
  ifs.close();

  // set default if option is invalid
  if (gTerrainOption > 4)
  {
    gTerrainOption = 2;
  }
  if (gObstaclesOption > 4)
  {
    gObstaclesOption = 2;
  }
  if (gWindOption > 3)
  {
    gWindOption = 1;
  }
  if (gFullScreenOption > 1)
  {
    gFullScreenOption = 0;
  }

  // set globals based on options
  switch(gTerrainOption)
  {
    case 0: gTerrainType = 0; break;
    case 1: gTerrainType = 1; break;
    case 2: gTerrainType = 2; break;
    case 3: gTerrainType = 3; break;
    case 4: gTerrainType = nrand(4); break;
    default: break;
  }
  switch(gObstaclesOption)
  {
    case 0: gObstacleTotal = 0; break;
    case 1: gObstacleTotal = 6; break;
    case 2: gObstacleTotal = 20; break;
    case 3: gObstacleTotal = 40; break;
    case 4: 
            {
              unsigned int obs_total = nrand(41);
              if (obs_total == 1) { obs_total = 2; }
              if (obs_total == 3) { obs_total = 4; }
              gObstacleTotal = obs_total;
              break;
            } 
    default: break;
  }
  switch(gWindOption)
  {
    case 0: gWind = 0; break;
    case 1: gWind = 1; break;
    case 2: gWind = 2; break;
    case 3: gWind = nrand(3); break;
    default: break;
  }
  switch(gFullScreenOption)
  {
    case 0: gFullScreen = 0; break;
    case 1: gFullScreen = 1; break;
    default: break;
  }
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
