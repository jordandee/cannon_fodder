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
  }
  ifs.close();
}

