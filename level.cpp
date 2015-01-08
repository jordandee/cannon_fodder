#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include "gameengine.h"
#include "terrain.h"
#include "resources.h"
#include "timer.h"
#include "cannon.h"
#include "ball.h"
#include "level.h"

Level Level::_s;

void Level::init(GameEngine* ge)
{
  timer.start();

  generateTerrain(terrain);

  cannonL.init(ge->renderer, false);

  // find placement for left cannon, top ground pixel at randomized x
  auto lt = std::find_if(terrain.begin(), terrain.end(), isTopPixelL);
  //std::cout << lt->x << " " << lt->y << " " << std::endl;

  cannonL.setPosition(lt->x, lt->y - CANNON_HEIGHT);

  // make sure cannon sits on terrain properly and does not overlap it
  fixTerrain(terrain, lt->x, lt->y);

  cannonR.init(ge->renderer, true);

  // find placement for right cannon
  auto rt = std::find_if(terrain.begin(), terrain.end(), isTopPixelR);
  //std::cout << rt->x << " " << rt->y << " " << std::endl;

  cannonR.setPosition(rt->x, rt->y - CANNON_HEIGHT);

  // make sure cannon sits on terrain properly and does not overlap it
  fixTerrain(terrain, rt->x, rt->y);

  ball.init(ge->renderer);
}

void Level::quit()
{
}

void Level::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
    if (e.type == SDL_QUIT)
      ge->stop();
    if (e.type == SDL_KEYDOWN)
      if (e.key.keysym.sym == SDLK_ESCAPE)
        ge->stop();

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
  {
    cannonL.incrementAngle(5.0);
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
  {
    cannonL.incrementAngle(-5.0);
  }

}

void Level::update()
{
  dt = timer.getTime();
  timer.start();

  ball.update(dt);
}

void Level::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  // draw terrain
  // for some reason won't draw black (0,0,0,0) on white...
  SDL_SetRenderDrawColor(ge->renderer, 0x00, 0x00, 0x00, 1);
  for (auto it = terrain.begin(); it != terrain.end(); ++it)
  {
    if (it->status)
    {
      SDL_RenderDrawPoint(ge->renderer, it->x, it->y);
    }
  }

  cannonL.render(ge->renderer);
  cannonR.render(ge->renderer);

  ball.render(ge->renderer);

  SDL_RenderPresent(ge->renderer);
}
