#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "gameengine.h"
#include "terrain.h"
#include "resources.h"
#include "level.h"

Level Level::_s;

void Level::init(GameEngine* ge)
{
  generateTerrain(terrain);

  cannon = NULL;
  cannon = loadTexture("images/cannon_wheel.png", ge->renderer);

  // find placement for left cannon
  auto lt = std::find_if(terrain.begin(), terrain.end(), isTopPixelL);
  std::cout << lt->x << " " << lt->y << " " << std::endl;
  cannon_rectL.w = 30;
  cannon_rectL.h = 24;
  cannon_rectL.x = lt->x;
  cannon_rectL.y = lt->y - cannon_rectL.h;

  // make sure cannon sits on terrain properly and does not overlap it
  fixTerrain(terrain, lt->x, lt->y);

  // find placement for right cannon
  auto rt = std::find_if(terrain.begin(), terrain.end(), isTopPixelR);
  std::cout << rt->x << " " << rt->y << " " << std::endl;
  cannon_rectR.w = 30;
  cannon_rectR.h = 24;
  cannon_rectR.x = rt->x;
  cannon_rectR.y = rt->y - cannon_rectR.h;

  // make sure cannon sits on terrain properly and does not overlap it
  fixTerrain(terrain, rt->x, rt->y);

  shaft = NULL;
  shaft = loadTexture("images/cannon_shaft.png", ge->renderer);

  shaft_rectL.w = 4;
  shaft_rectL.h = 14;
  shaft_rectL.x = cannon_rectL.x + 17;
  shaft_rectL.y = cannon_rectL.y + 3;

  shaft_rectR.w = 4;
  shaft_rectR.h = 14;
  shaft_rectR.x = cannon_rectR.x + 9;
  shaft_rectR.y = cannon_rectR.y + 3;
}

void Level::quit()
{
  SDL_DestroyTexture(cannon);
}

void Level::handleEvents(GameEngine* ge)
{
  SDL_Event e;
    if (SDL_PollEvent(&e))
      if (e.type == SDL_QUIT)
        ge->stop();
      if (e.type == SDL_KEYDOWN)
        if (e.key.keysym.sym == SDLK_ESCAPE)
          ge->stop();
}

void Level::update()
{
}

void Level::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  //SDL_SetRenderDrawColor(ge->renderer, 0x00, 0x00, 0x00, 0);
  SDL_RenderClear(ge->renderer);

  // for some reason won't draw black (0,0,0,0) on white...
  SDL_SetRenderDrawColor(ge->renderer, 0x00, 0x00, 0x00, 1);
  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->status)
    {
      SDL_RenderDrawPoint(ge->renderer, it->x, it->y);
    }
  }

  SDL_RenderCopy(ge->renderer, cannon, NULL, &cannon_rectL);
  SDL_RenderCopyEx(ge->renderer, cannon, NULL, &cannon_rectR, 0, NULL, SDL_FLIP_HORIZONTAL);

  SDL_RenderCopy(ge->renderer, shaft, NULL, &shaft_rectL);
  SDL_RenderCopy(ge->renderer, shaft, NULL, &shaft_rectR);

  SDL_RenderPresent(ge->renderer);
}
