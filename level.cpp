#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include "gameengine.h"
#include "resources.h"
#include "level.h"

Level Level::_s;

void Level::init(GameEngine* ge)
{
  generateTerrain(terrain);

  cannon = NULL;
  cannon = loadTexture("images/cannon.png", ge->renderer);
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

  SDL_Rect dest;
  dest.x = 100;
  dest.y = 100;
  dest.w = 30;
  dest.h = 24;
  SDL_RenderCopy(ge->renderer, cannon, NULL, &dest);

  SDL_RenderPresent(ge->renderer);
}
