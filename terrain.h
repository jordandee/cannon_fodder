#ifndef TERRAIN_H
#define TERRAIN_H

#include <SDL2/SDL.h>
#include <vector>

struct Pixel
{
  int x;
  int y;

  int status; // 0: air, 1: dirt
};

void generateTerrain(std::vector<Pixel>& terrain);

int findTopGroundPixel(std::vector<Pixel>& terrain, int x);
void fixTerrain(std::vector<Pixel>& terrain, SDL_Rect* rect);

bool checkTerrainCollision(SDL_Rect* ball, std::vector<Pixel>& terrain);
void explodeTerrain(int x, int y, std::vector<Pixel>& terrain);

#endif
