#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "terrain.h"
#include "math.h"
#include "globals.h"

void generateTerrain(std::vector<Pixel>& terrain)
{
  const double YFLOOR = 575.0;
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
  else if (gTerrainType == 1) // smooth - double slope
  {
    double s1 = -1.0/((double)nrand(3)+2.0), s2 = 1.0/((double)nrand(3)+2.0);
    double y1 = 400, x1 = 0, y2 = 300, x2 = 250 + nrand(400);

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
  else if (gTerrainType == 2) // quadruple slope with randomization
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
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x3)
      {
        double y = y2 + s2 * ((double)it->x - x2);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x4)
      {
        double y = y3 + s3 * ((double)it->x - x3);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else
      {
        double y = y4 + s4 * ((double)it->x - x4);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
    }
  }
  else if (gTerrainType == 3) // extreme - 8 slopes, likely to be steep
  {
    double s1 = -8.0/((double)nrand(12) + 1.0);
    double x1 = 0;
    double y1 = 350 + nrand(150);

    double s2 = 8.0/((double)nrand(12) + 1.0);
    double x2 = 50 + nrand(100);
    double y2 = (y1 + s1 * ((double)x2 - x1));
    if (y2 < 300) y2 = 300;
    if (y2 > 550) y2 = 550;

    double s3 = -8.0/((double)nrand(12) + 1.0);
    double x3 = 150 + nrand(100);
    double y3 = y2 + s2 * ((double)x3 - x2);
    if (y3 < 300) y3 = 300;
    if (y3 > 550) y3 = 550;

    double s4 = 8.0/((double)nrand(12) + 1.0);
    double x4 = 250 + nrand(100);
    double y4 = y3 + s3 * ((double)x4 - x3);
    if (y4 < 300) y4 = 300;
    if (y4 > 550) y4 = 550;

    double s5 = -8.0/((double)nrand(12) + 1.0);
    double x5 = 350 + nrand(100);
    double y5 = y4 + s4 * ((double)x5 - x4);
    if (y5 < 300) y5 = 300;
    if (y5 > 550) y5 = 550;

    double s6 = 8.0/((double)nrand(12) + 1.0);
    double x6 = 450 + nrand(100);
    double y6 = y5 + s5 * ((double)x6 - x5);
    if (y6 < 300) y6 = 300;
    if (y6 > 550) y6 = 550;

    double s7 = -8.0/((double)nrand(12) + 1.0);
    double x7 = 550 + nrand(100);
    double y7 = y6 + s6 * ((double)x7 - x6);
    if (y7 < 300) y7 = 300;
    if (y7 > 550) y7 = 550;

    double s8 = 8.0/((double)nrand(12) + 1.0);
    double x8 = 650 + nrand(100);
    double y8 = y7 + s7 * ((double)x8 - x7);
    if (y8 < 300) y8 = 300;
    if (y8 > 550) y8 = 550;

    for (auto it = terrain.begin(); it != terrain.end(); ++it)
    {
      if (it->x < x2)
      {
        double y = y1 + s1 * ((double)it->x - x1);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x3)
      {
        double y = y2 + s2 * ((double)it->x - x2);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x4)
      {
        double y = y3 + s3 * ((double)it->x - x3);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x5)
      {
        double y = y4 + s4 * ((double)it->x - x4);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x6)
      {
        double y = y5 + s5 * ((double)it->x - x5);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x7)
      {
        double y = y6 + s6 * ((double)it->x - x6);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else if (it->x < x8)
      {
        double y = y7 + s7 * ((double)it->x - x7);
        y = fmin(y, YFLOOR);
        if ((int)y >= it->y)
          it->status = 0;
      }
      else
      {
        double y = y8 + s8 * ((double)it->x - x8);
        y = fmin(y, YFLOOR);
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
void fixTerrain(std::vector<Pixel>& terrain, SDL_Rect* rect)
{
  int bx = rect->x;
  int by = rect->y + rect->h;
  int bw = rect->w;

  // only fix terrain under tree trunk
  const int TREE_WIDTH = 13;
  if (bw == TREE_WIDTH)
  {
    bx += 5;
    bw = 3;
  }

  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->x >= bx && it->x < bx + bw)
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

