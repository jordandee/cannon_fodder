#include <vector>
#include "terrain.h"

void generateTerrain(std::vector<Pixel>& terrain)
{
  int type = 2;
  // populate terrain vector with default flat terrain
  for (int y = 300; y < 600; ++y)
  {
    for (int x = 0; x < 800; ++x)
    {
      Pixel p;
      p.x = x;
      p.y = y;
      p.status = 1;

      terrain.push_back(p);
    }
  }

  if (type == 0) //flat
    return;
  else if (type == 1) //single slope
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
  else if (type == 2) //double slope
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

}
