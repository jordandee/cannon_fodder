#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "gameengine.h"
#include "gamestate.h"
#include "resources.h"

void GameEngine::init()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);

  win = SDL_CreateWindow("Cannon Fodder", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
  renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

  loadSettings();

  IMG_Init(IMG_INIT_PNG);

  TTF_Init();
}

void GameEngine::quit()
{
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(win);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}

void GameEngine::changeState(GameState* gamestate)
{
  if (gs != NULL)
    gs->quit();
  gs = gamestate;
  gs->init(this);
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
