#include "SDL2/SDL.h"
#include "gameengine.h"
#include "level.h"

Level Level::_s;

void Level::init()
{
}

void Level::quit()
{
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
  SDL_RenderClear(ge->renderer);
  SDL_RenderPresent(ge->renderer);
}
