#include "SDL2/SDL.h"
#include "gameengine.h"
#include "gamestate.h"
#include "resources.h"

void GameEngine::init()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  win = SDL_CreateWindow("Cannon Fodder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  loadSettings();
}

void GameEngine::quit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  SDL_Quit();
}

void GameEngine::changeState(GameState* gamestate)
{
  if (gs != NULL)
    gs->quit();
  gs = gamestate;
  gs->init();
}

void GameEngine::handleEvents()
{
  gs->handleEvents(this);
}

void GameEngine::update()
{
  gs->update();
}

void GameEngine::render()
{
  gs->render(this);
}
