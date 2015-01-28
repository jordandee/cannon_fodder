#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include "gameengine.h"
#include "terrain.h"
#include "resources.h"
#include "timer.h"
#include "cannon.h"
#include "ball.h"
#include "score.h"
#include "math.h"
#include "level.h"

Level Level::_s;

void Level::init(GameEngine* ge)
{
  timer.start();

  cannonL.init(ge->renderer, false);
  cannonR.init(ge->renderer, true);

  spawnLevel();

  ball.init(ge->renderer);

  shooting = false;
  shot_dt = 0.0;
  keyup_frames = 0;

  force_texture = NULL;
  force_texture = loadTexture("images/cannon_ball.png", ge->renderer);

  force_rect.w = 4;
  force_rect.h = 16;
  force_rect.x = 10;
  force_rect.y = 20;

  is_player1 = true;

  score.init(ge->renderer);
}

void Level::spawnLevel()
{
  generateTerrain(terrain);

  cannonL.live();
  cannonR.live();
  is_a_player_dead = false;

  // find placement for left cannon, top ground pixel at randomized x
  int cannonL_x = 150 + nrand(100);
  auto lt = std::find_if(terrain.begin(), terrain.end(),
      [&cannonL_x](Pixel p)
      {
        if (p.x == cannonL_x && p.status)
          return true;
        else
          return false;
      });

  cannonL.setPosition(lt->x, lt->y - CANNON_HEIGHT);
  fixTerrain(terrain, lt->x, lt->y);

  // find placement for right cannon, top ground pixel at randomized x
  int cannonR_x = 550 + nrand(100);
  auto rt = std::find_if(terrain.begin(), terrain.end(),
      [&cannonR_x](Pixel p)
      {
        if (p.x == cannonR_x && p.status)
          return true;
        else
          return false;
      });

  cannonR.setPosition(rt->x, rt->y - CANNON_HEIGHT);
  fixTerrain(terrain, rt->x, rt->y);
}

void Level::quit()
{
  SDL_DestroyTexture(force_texture);
}

void Level::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      ge->stop();
    if (e.type == SDL_KEYDOWN)
    {
      if (e.key.keysym.sym == SDLK_ESCAPE)
        ge->stop();
    }
  }

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
  {
    if (ball.isDead())
    {
      if (is_player1)
        cannonL.incrementAngle(5.0);
      else
        cannonR.incrementAngle(5.0);
    }
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
  {
    if (ball.isDead())
    {
      if (is_player1)
        cannonL.incrementAngle(-5.0);
      else
        cannonR.incrementAngle(-5.0);
    }
  }
  if (state[SDL_SCANCODE_SPACE])
  {
    if (ball.isDead())
    {
      if (!shooting)
      {
        shooting = true;
        shot_dt = 0.0;
        shot_timer.start();
      }
      else
      {
        shot_dt = shot_timer.getTime();
      }
    }
  }
  else
  {
    if (shooting)
    {
      keyup_frames++; // synergy hack, synergy creates false positive keyups
      if (keyup_frames >= 5)
      {
        shooting = false;
        shot_dt = shot_timer.getTime();
        //std::cout << "shot_dt: " << shot_dt << "\n";
        keyup_frames = 0;

        if (is_player1)
          ball.shoot(cannonL.getCX(), cannonL.getCY(), shot_dt, cannonL.getAngle());
        else
          ball.shoot(cannonR.getCX(), cannonR.getCY(), shot_dt, cannonR.getAngle());
        shot_dt = 0.0;
      }
    }
  }
}

void Level::update()
{
  dt = timer.getTime();
  timer.start();

  if (!ball.isDead())
  {
    ball.update(dt);
    if (ball.checkTerrainCollision(terrain))
    {
      is_player1 = !is_player1;
    }

    if (is_player1)
    {
      if (ball.checkCannonCollision(cannonR.getRect()))
      {
        cannonR.die();
        score.playerLScores(10);
        is_player1 = !is_player1;
        respawn_timer.start();
        is_a_player_dead = true;
      }
    }
    else
    {
      if (ball.checkCannonCollision(cannonL.getRect()))
      {
        cannonL.die();
        score.playerRScores(10);
        is_player1 = !is_player1;
        respawn_timer.start();
        is_a_player_dead = true;
      }
    }
  }

  const double min_shot_dt = .4;
  if (shot_dt < min_shot_dt)
    force_rect.w = min_shot_dt * 64;
  else
    force_rect.w = shot_dt * 64;

  if (is_player1)
    force_rect.x = 10;
  else
    force_rect.x = 790 - force_rect.w;
}

void Level::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  if (is_a_player_dead && respawn_timer.getTime() > 3.0)
    spawnLevel();

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

  if (shooting)
    SDL_RenderCopy(ge->renderer, force_texture, NULL, &force_rect);

  score.render(ge->renderer);

  SDL_RenderPresent(ge->renderer);
}
