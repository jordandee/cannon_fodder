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
#include "obstacle.h"
#include "globals.h"
#include "title.h"
#include "level.h"

Level Level::_s;

void Level::init(GameEngine* ge)
{
  timer.start();

  cannonL.init(ge->renderer, false);
  cannonR.init(ge->renderer, true);

  int max_obstacles = 40;
  if (obstacles.empty())
  {
    for (int i = 0; i < max_obstacles; i++)
    {
      Obstacle obs;
      obstacles.push_back(obs);
    }
    // why? have to init after pushing obstacle onto vector
    //   otherwise one or more obstacles won't render
    for (int i = 0; i < max_obstacles; ++i)
    {
      bool is_flipped = (i % 2 == 1);
      Obstacle_Type obstacle_type;
      switch (i)
      {
        case 0: case 1: obstacle_type = HOSPITAL; break;
        case 2: case 3: obstacle_type = HOUSE; break;
        default: obstacle_type = TREE; break;
      }
      obstacles[i].init(ge->renderer, obstacle_type, is_flipped);
    }
  }


  if (!points)
  {
    points = (SDL_Point *)calloc(800*300,sizeof(SDL_Point));
    point_count = 800*300;
  }

  spawnLevel();

  ball.init(ge->renderer);

  shooting = false;
  shot_dt = 0.0;
  keyup_frames = 0;

  if (!force_texture)
  {
    force_texture = loadTexture("images/cannon_ball.png", ge->renderer);
  }

  force_rect.w = 4;
  force_rect.h = 16;
  force_rect.x = 10;
  force_rect.y = 20;

  is_player1 = true;

  score.init(ge->renderer);
  win_score = 25;
}

void Level::spawnLevel()
{
  setupSettingsBasedGlobals();
  
  generateTerrain(terrain);

  cannonL.live();
  cannonR.live();
  is_a_player_dead = false;

  // find placement for left cannon, top ground pixel at randomized x
  int cannonL_cx = 100 + nrand(200); // center x of cannon
  auto lt = std::find_if(terrain.begin(), terrain.end(),
      [&cannonL_cx](Pixel p)
      {
        if (p.x == cannonL_cx && p.status)
          return true;
        else
          return false;
      });

  cannonL.setPosition(cannonL_cx - CANNON_WIDTH/2, lt->y - CANNON_HEIGHT);
  fixTerrain(terrain, cannonL.getRect());

  // find placement for right cannon, top ground pixel at randomized x
  int cannonR_cx = 500 + nrand(200);
  auto rt = std::find_if(terrain.begin(), terrain.end(),
      [&cannonR_cx](Pixel p)
      {
        if (p.x == cannonR_cx && p.status)
          return true;
        else
          return false;
      });

  cannonR.setPosition(cannonR_cx - CANNON_WIDTH/2, rt->y - CANNON_HEIGHT);
  fixTerrain(terrain, cannonR.getRect());


  // find positions for obstacles
  // make sure they don't collide with cannons or themselves
  std::vector<SDL_Rect *> rects;
  rects.push_back(cannonL.getRect());
  rects.push_back(cannonR.getRect());
  for (int i = 0; i < gObstacleTotal; i++)
  {
    obstacles[i].findPosition(terrain, rects);
    if (i < 10) // don't worry about trees colliding after 10+ obstacles
      rects.push_back(obstacles[i].getRect());
  }
    
  missed_left = false;
  missed_right = false;
  missed_distance = 0;
  last_ai_angle = -60.0;
  last_ai_shot_dt = 1.0;
  
  // copy terrain vector to SDL_Points array for faster rendering
  copyTerrainToPointsArray();
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
    {
      ge->stop();
    }
    if (e.type == SDL_KEYDOWN)
    {
      if (e.key.keysym.sym == SDLK_ESCAPE)
      {
        ge->changeState(Title::Instance());
      }
    }
  }

  const Uint8* state = SDL_GetKeyboardState(NULL);

  if (gAI_Enable && !is_player1 && ball.isDead() && !is_a_player_dead)
  {
    double ai_angle = (-(double)nrand(60)) - 15.0;
    double ai_shot_dt = ((double) nrand(100))/50.0 + 1.0; 
    double ai_increment = ((double)missed_distance)/500.0;
    if (missed_right)
    {
      // if ai keeps guessing close, randomize angle again, otherwise use last angle
      if (ai_increment >= .04)
      {
        ai_angle = last_ai_angle;
      }
      ai_shot_dt = last_ai_shot_dt + ai_increment;
      missed_right = false;
    }
    if (missed_left)
    {
      if (ai_increment >= .04)
      {
        ai_angle = last_ai_angle;
      }
      ai_shot_dt = last_ai_shot_dt - ai_increment;
      missed_left = false;
    }
    //std::cout << ai_angle << " " << ai_shot_dt << " " << ai_increment << std::endl;

    cannonR.setAngle(ai_angle);
    ball.shoot(cannonR.getCX(), cannonR.getCY(), ai_shot_dt, cannonR.getAngle());
    last_ai_angle = ai_angle;
    last_ai_shot_dt = ai_shot_dt;
  }

  if (state[SDL_SCANCODE_D] || state[SDL_SCANCODE_RIGHT])
  {
    if (ball.isDead())
    {
      if (is_player1)
        cannonL.incrementAngle(180.0*dt);
      else
        cannonR.incrementAngle(180.0*dt);
    }
  }
  if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_LEFT])
  {
    if (ball.isDead())
    {
      if (is_player1)
        cannonL.incrementAngle(-180.0*dt);
      else
        cannonR.incrementAngle(-180.0*dt);
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
        keyup_frames = 0;
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
  //std::cout << "dt: " << dt << std::endl;

  if (!ball.isDead())
  {
    ball.update(dt);

    if (ball.getRect()->y > 270) // don't bother with collision detecion if ball is in sky
    {
      if (is_player1)
      {
        if (ball.checkTerrainCollision(terrain))
        {
          copyTerrainToPointsArray();
          is_player1 = !is_player1;
        }
        else if (ball.checkRectCollision(cannonR.getRect()))
        {
          cannonR.die();
          score.playerLScores(10);
          is_player1 = !is_player1;
          respawn_timer.start();
          is_a_player_dead = true;
          if (score.getPlayerLScore() >= win_score)
            score.warMessage = 1;
          else
            score.battleMessage = 1;
        }
        else if (gObstacleTotal >= 2 && obstacles[1].alive && ball.checkRectCollision(obstacles[1].getRect()))
        {
          obstacles[1].alive = false;
          score.playerLScores(5);
          is_player1 = !is_player1;
        }
        else if (gObstacleTotal >= 4 && obstacles[3].alive && ball.checkRectCollision(obstacles[3].getRect()))
        {
          obstacles[3].alive = false;
          score.playerLScores(3);
          is_player1 = !is_player1;
        }
      }
      else if (!is_player1)
      {
        if (ball.checkTerrainCollision(terrain))
        {
          if (gAI_Enable)
          {
            if (ball.getRect()->x + ball.getRect()->w/2 < cannonL.getCX())
            {
              missed_distance = cannonL.getCX() - (ball.getRect()->x + ball.getRect()->w/2);
              missed_left = true;
            }
            else
            {
              missed_distance = (ball.getRect()->x + ball.getRect()->w/2) - cannonL.getCX();
              missed_right = true;
            }
          }
          copyTerrainToPointsArray();
          is_player1 = !is_player1;
        }
        else if (ball.checkRectCollision(cannonL.getRect()))
        {
          cannonL.die();
          score.playerRScores(10);
          is_player1 = !is_player1;
          respawn_timer.start();
          is_a_player_dead = true;
          if (score.getPlayerRScore() >= win_score)
            score.warMessage = 2;
          else
            score.battleMessage = 2;
        }
        else if (gObstacleTotal >= 2 && obstacles[0].alive && ball.checkRectCollision(obstacles[0].getRect()))
        {
          obstacles[0].alive = false;
          score.playerRScores(5);
          is_player1 = !is_player1;
        }
        else if (gObstacleTotal >= 4 && obstacles[2].alive && ball.checkRectCollision(obstacles[2].getRect()))
        {
          obstacles[2].alive = false;
          score.playerRScores(3);
          is_player1 = !is_player1;
        }
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
  Timer rendertime;
  //rendertime.start();
  
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  if (is_a_player_dead && (respawn_timer.getTime() > 3.0f))
  {
    spawnLevel();
    score.battleMessage = 0;
    if (score.warMessage != 0)
    {
      score.resetScores();
      score.warMessage = 0;
    }
  }

  for (int i = 0; i < gObstacleTotal; i++)
  {
    obstacles[i].render(ge->renderer);
  }
  
  // draw terrain
  SDL_SetRenderDrawColor(ge->renderer, 0x00, 0x00, 0x00, 1);
  SDL_RenderDrawPoints(ge->renderer, points, point_count);

  cannonL.render(ge->renderer);
  cannonR.render(ge->renderer);

  ball.render(ge->renderer);

  if (shooting)
    SDL_RenderCopy(ge->renderer, force_texture, NULL, &force_rect);

  score.render(ge->renderer);

  SDL_RenderPresent(ge->renderer);

  //std::cout << rendertime.getTime() << std::endl;
}
  
// copy terrain vector to SDL_Points array for faster rendering
void Level::copyTerrainToPointsArray()
{
  int i = 0;
  point_count = 0;
  while(i < 300*800)
  {
    if (terrain[i].status)
    {
      points[point_count].x = terrain[i].x;
      points[point_count].y = terrain[i].y;
      ++point_count;
    }
    ++i;
  }
}
