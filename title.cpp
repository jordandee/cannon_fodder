#include "SDL2/SDL.h"
#include "gameengine.h"
#include "title.h"
#include "level.h"

Title Title::_s;

void Title::init()
{
}

void Title::quit()
{
}

void Title::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  if (SDL_PollEvent(&e))
    if (e.type == SDL_QUIT)
      ge->stop();
    if (e.type == SDL_KEYDOWN)
      if (e.key.keysym.sym == SDLK_ESCAPE)
        ge->stop();
      else
        ge->changeState(Level::Instance());
}

void Title::update()
{
}

void Title::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 0);
  SDL_RenderClear(ge->renderer);
  SDL_RenderPresent(ge->renderer);
}
