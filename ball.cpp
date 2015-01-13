#include <SDL2/SDL.h>
#include <cmath>
#include "resources.h"
#include "ball.h"

#define PI 3.14159265

Ball::Ball()
{
  ball_rect.w = BALL_WIDTH;
  ball_rect.h = BALL_HEIGHT;
  ball_rect.x = 100;
  ball_rect.y = 100;
}

Ball::~Ball()
{
  SDL_DestroyTexture(ball_texture);
}

void Ball::init(SDL_Renderer* renderer)
{
  ball_texture = NULL;
  ball_texture = loadTexture("images/cannon_ball.png", renderer);

  x = 100.0;
  vx = 80.0;
  ax = -20.0;

  y = 100.0;
  vy = 0.0;
  ay = 20.0;

  ball_angle = 0;
  ball_rotation_point.x = BALL_WIDTH/2;
  ball_rotation_point.y = BALL_HEIGHT/2;
}

void Ball::update(double dt)
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

void Ball::render(SDL_Renderer* renderer)
{
  SDL_RenderCopyEx(renderer, ball_texture, NULL, &ball_rect, ball_angle, &ball_rotation_point, SDL_FLIP_NONE);
}

void Ball::setPosition(int x, int y)
{
}

void Ball::incrementAngle(double a)
{
}

void Ball::shoot(int cannon_cx, int cannon_cy, double shot_dt, double shot_angle)
{
  x = cannon_cx + (BALL_WIDTH/2);
  y = cannon_cy + (BALL_HEIGHT/2);
  ball_rect.x = x;
  ball_rect.y = y;

  double vr = shot_dt * 100.0;

  vx = vr * sin(shot_angle * PI/180.0);
  vy = -vr * cos(shot_angle * PI/180.0);
  ax = 0.0;
  ay = 50.0;

  ball_angle = shot_angle;
}
