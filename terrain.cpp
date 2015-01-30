#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "terrain.h"
#include "math.h"
#include "globals.h"

void generateTerrain(std::vector<Pixel>& terrain)
{
  // populate terrain vector with default flat terrain
  bool create_vector = terrain.empty();
  int i = 0;
  for (int y = 300; y < 600; ++y)
  {
    for (int x = 0; x < 800; ++x)
    {
      Pixel p;
      p.x = x;
      p.y = y;
      p.status = 1;

      if (create_vector)
        terrain.push_back(p);
      else
        terrain[i++] = p;
    }
  }

  if (gTerrainType == 0) //flat
    return;
  else if (gTerrainType == 1) //single slope
  {
    // single slope terrain
    double slope = -1.0/8.0;
    double y1 = 350, x1 = 0;

    // if current y is above slope line defined above, make it white
    for (auto it = terrain.begin(); it != terrain.end(); ++it)
    {
      double y = y1 + slope * ((double)it->x - x1);
      if ((int)y >= it->y)
        it->status = 0;
    }
  }
  else if (gTerrainType == 2) //double slope
  {
    double s1 = -1.0/4.0, s2 = 1.0/4.0;
    double y1 = 400, x1 = 0, y2 = 300, x2 = 400;

    for (auto it = terrain.begin(); it != terrain.end(); ++it)
    {
      if (it->x < x2)
      {
        double y = y1 + s1 * ((double)it->x - x1);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else
      {
        double y = y2 + s2 * ((double)it->x - x2);
        if ((int)y >= it->y)
          it->status = 0;
      }
    }
  }
  else if (gTerrainType == 3) // quadruple slope
  {
    double s1 = -1.0/4.0, s2 = 1.0/4.0;
    double s3 = -1.0/4.0, s4 = 1.0/4.0;
    double y1 = 400, x1 = 0, y2 = 300, x2 = 200;
    double y3 = 400, x3 = 400, y4 = 300, x4 = 600;

    for (auto it = terrain.begin(); it != terrain.end(); ++it)
    {
      if (it->x < x2)
      {
        double y = y1 + s1 * ((double)it->x - x1);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x3)
      {
        double y = y2 + s2 * ((double)it->x - x2);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x4)
      {
        double y = y3 + s3 * ((double)it->x - x3);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else
      {
        double y = y4 + s4 * ((double)it->x - x4);
        if ((int)y >= it->y)
          it->status = 0;
      }
    }
  }
  else if (gTerrainType == 4) // quadruple slope with randomization
  {
    double s1 = -1.0/((double)nrand(8) + 1.0);
    double x1 = 0;
    double y1 = 350 + nrand(150);

    double s2 = 1.0/((double)nrand(8) + 1.0);
    double x2 = 100 + nrand(200);
    double y2 = (y1 + s1 * ((double)x2 - x1));
    if (y2 < 300) y2 = 300;
    if (y2 > 550) y2 = 550;

    double s3 = -1.0/((double)nrand(8) + 1.0);
    double x3 = 300 + nrand(200);
    double y3 = y2 + s2 * ((double)x3 - x2);
    if (y3 < 300) y3 = 300;
    if (y3 > 550) y3 = 550;

    double s4 = 1.0/((double)nrand(8) + 1.0);
    double x4 = 500 + nrand(200);
    double y4 = y3 + s3 * ((double)x4 - x3);
    if (y4 < 300) y4 = 300;
    if (y4 > 550) y4 = 550;

    for (auto it = terrain.begin(); it != terrain.end(); ++it)
    {
      if (it->x < x2)
      {
        double y = y1 + s1 * ((double)it->x - x1);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x3)
      {
        double y = y2 + s2 * ((double)it->x - x2);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x4)
      {
        double y = y3 + s3 * ((double)it->x - x3);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else
      {
        double y = y4 + s4 * ((double)it->x - x4);
        if ((int)y >= it->y)
          it->status = 0;
      }
    }
  }
}

int findTopGroundPixel(std::vector<Pixel>& terrain, int x)
{
  int y = 0;
  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->x == x && it->status)
    {
      y = it->y;
      break;
    }
  }
  return y;
}

// make sure cannon sits on terrain properly and does not overlap it
//  take bottom left cannon coordinates
//  get rid of overlapping dirt and cannon
//  make sure cannon sits on dirt
void fixTerrain(std::vector<Pixel>& terrain, int bx, int by)
{
  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->x >= bx && it->x < bx + 30)
    {
      if (it->y < by)
        it->status = 0;
      else
        it->status = 1;
    }
  }
}

bool checkTerrainCollision(SDL_Rect* ball, std::vector<Pixel>& terrain)
{
  bool collision_detected = false;

  auto it = terrain.begin();
  while (it != terrain.end() && !collision_detected)
  {
    while (!it->status)
      ++it;

    // check if ball center is on dirt block
    if (ball->x+2 == it->x && ball->y+2 == it->y)
    {
      collision_detected = true;

      explodeTerrain(it->x, it->y, terrain);
    }
    ++it;
  }

  return collision_detected;
}

void explodeTerrain(int x, int y, std::vector<Pixel>& terrain)
{
  double cx = (double) x;
  double cy = (double) y;

  const double explode_radius = 20.0;
  double radius;

  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->status)
    {
      radius = sqrt( (cx - (double)it->x)*(cx - (double)it->x) +
                     (cy - (double)it->y)*(cy - (double)it->y) );
      if (radius < explode_radius)
        it->status = 0;
    }
  }
}

