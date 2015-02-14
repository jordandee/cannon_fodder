#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "resources.h"
#include "terrain.h"
#include "globals.h"
#include "ball.h"
#include "math.h"

#define PI 3.14159265
#define abs(x) (((x) < 0) ? -x : x)

Ball::Ball()
{
  ball_rect.w = BALL_WIDTH;
  ball_rect.h = BALL_HEIGHT;
  ball_rotation_point.x = BALL_WIDTH/2;
  ball_rotation_point.y = BALL_HEIGHT/2;
}

Ball::~Ball()
{
  SDL_DestroyTexture(ball_texture);
}

void Ball::init(SDL_Renderer* renderer)
{
  if (!ball_texture)
  {
    ball_texture = loadTexture("images/cannon_ball.png", renderer);
  }

  alive = false;
  
  ball_rect.x = 0;
  ball_rect.y = 0;

  x = 0.0;
  vx = 0.0;
  ax = 0.0;

  y = 0.0;
  vy = 0.0;
  ay = 0.0;

  ball_angle = 0;
}

void Ball::update(double dt)
{
  if (alive)
  {
    // simplified wind resistance, bad physics with lots of magic numbers
    double wa = 0.0;
    if (gWindOption == 0) // no wind resistance
    {
      vx = vx + (ax * dt);
      vy = vy + (ay * dt);
    }
    else // wind resistance
    {
      double scale = 1.0/4.0;
      if (vx >= 0.0 && gWind >= 0.0 && abs(vx) < abs(1.4*gWind))
      {
        wa = (gWind+vx) * scale * dt;
      }
      else if (vx >= 0.0 && gWind < 0.0)
      {
        wa = (gWind-vx) * scale *dt;
      }
      else if (vx < 0.0 && gWind < 0.0 && abs(vx) < abs(1.4*gWind))
      {
        wa = (gWind+vx) * scale * dt;
      }
      else if (vx < 0.0 && gWind >= 0.0)
      {
        wa = (gWind-vx) * scale *dt;
      }
     
      vx = vx + (ax * dt) + wa;
      vy = vy + (ay * dt) + (vy * -1/16 * dt); // just air resistance in y direction
    }
    //std::cout << vx << "  " << gWind << "  " << wa << std::endl;

    x = x + vx * dt + .5 * ax * dt * dt;
    y = y + vy * dt + .5 * ay * dt * dt;

    ball_rect.x = (int) x;
    ball_rect.y = (int) y;

    // rotate ball 180 degrees/sec unless it's going straight up/down
    if (vx > 0)
      ball_angle += 180.0 * dt;
    else if (vx < 0)
      ball_angle -= 180.0 * dt;
  }
}

void Ball::render(SDL_Renderer* renderer)
{
  if (alive)
    SDL_RenderCopyEx(renderer, ball_texture, NULL, &ball_rect, ball_angle, &ball_rotation_point, SDL_FLIP_NONE);
}

void Ball::shoot(int cannon_cx, int cannon_cy, double shot_dt, double shot_angle)
{
  x = cannon_cx + (BALL_WIDTH/2);
  y = cannon_cy + (BALL_HEIGHT/2);
  ball_rect.x = (int) x;
  ball_rect.y = (int) y;

  const double min_shot_dt = .4;
  if (shot_dt < min_shot_dt)
    shot_dt = min_shot_dt;

  double vr = shot_dt * 100.0;

  vx = vr * sin(shot_angle * PI/180.0);
  vy = -vr * cos(shot_angle * PI/180.0);
  ax = 0.0;
  ay = 100.0;

  ball_angle = shot_angle;

  alive = true;
}

bool Ball::checkTerrainCollision(std::vector<Pixel>& terrain)
{
  bool collision_detected = false;

  // check screen border first
  if (x + BALL_WIDTH < 0 || x > 800 || y > 600)
  {
    collision_detected = true;
    alive = false;
  }

  auto it = terrain.begin();
  while (it != terrain.end() && !collision_detected)
  {
    while (!it->status)
      ++it;

    // check if ball center is on dirt block
    if (ball_rect.x+BALL_WIDTH/2 == it->x && 
        ball_rect.y+BALL_HEIGHT/2 == it->y)
    {
      collision_detected = true;

      alive = false;

      explodeTerrain(it->x, it->y, terrain);
    }
    ++it;
  }

  return collision_detected;
}

bool Ball::checkRectCollision(SDL_Rect* rect)
{
  bool collision_detected = true;

  if (ball_rect.x + ball_rect.w < rect->x)
    collision_detected = false;
  if (ball_rect.y + ball_rect.h < rect->y)
    collision_detected = false;
  if (ball_rect.x > rect->x + rect->w)
    collision_detected = false;
  if (ball_rect.y > rect->y + rect->h)
    collision_detected = false;

  if (collision_detected)
    alive = false;

  return collision_detected;
}

bool Ball::isDead()
{
  return !alive;
}

SDL_Rect *Ball::getRect()
{
  return &ball_rect;
}
