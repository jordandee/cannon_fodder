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

#endif
