#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include "resources.h"
#include "terrain.h"
#include "ball.h"

#define PI 3.14159265

Ball::Ball()
{
  alive = false;

  ball_rect.w = BALL_WIDTH;
  ball_rect.h = BALL_HEIGHT;
  ball_rect.x = 0;
  ball_rect.y = 0;
}

Ball::~Ball()
{
  SDL_DestroyTexture(ball_texture);
}

void Ball::init(SDL_Renderer* renderer)
{
  ball_texture = NULL;
  ball_texture = loadTexture("images/cannon_ball.png", renderer);

  x = 0.0;
  vx = 0.0;
  ax = 0.0;

  y = 0.0;
  vy = 0.0;
  ay = 0.0;

  ball_angle = 0;
  ball_rotation_point.x = BALL_WIDTH/2;
  ball_rotation_point.y = BALL_HEIGHT/2;
}

void Ball::update(double dt)
{
  if (alive)
  {
    x = x + vx * dt + ax * dt * dt;
    vx = vx + ax * dt;

    y = y + vy * dt + ay * dt * dt;
    vy = vy + ay * dt;

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
  ay = 50.0;

  ball_angle = shot_angle;

  alive = true;
}

bool Ball::checkTerrainCollision(std::vector<Pixel>& terrain)
{
  bool collision_detected = false;

  // check screen border first
  if (x + BALL_WIDTH < 0 || x > 800)
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

bool Ball::checkCannonCollision(SDL_Rect* cannon_rect)
{
  bool collision_detected = true;

  if (ball_rect.x + ball_rect.w < cannon_rect->x)
    collision_detected = false;
  if (ball_rect.y + ball_rect.h < cannon_rect->y)
    collision_detected = false;
  if (ball_rect.x > cannon_rect->x + cannon_rect->w)
    collision_detected = false;
  if (ball_rect.y > cannon_rect->y + cannon_rect->h)
    collision_detected = false;

  if (collision_detected)
    alive = false;

  return collision_detected;
}

bool Ball::isDead()
{
  return !alive;
}
