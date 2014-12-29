#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>

struct Pixel
{
  int x;
  int y;

  int status;
};

void generateTerrain(std::vector<Pixel>& terrain);

bool isTopPixelL(Pixel p);
bool isTopPixelR(Pixel p);

void fixTerrain(std::vector<Pixel>& terrain, int bx, int by);

#endif
