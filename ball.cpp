#include <SDL2/SDL.h>
#include "resources.h"
#include "ball.h"

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
}

void Ball::update(double dt)
{
  x = x + vx * dt + ax * dt * dt;
  vx = vx + ax * dt;

  y = y + vy * dt + ay * dt * dt;
  vy = vy + ay * dt;

  ball_rect.x = (int) x;
  ball_rect.y = (int) y;
}

void Ball::render(SDL_Renderer* renderer)
{
  SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
}

void Ball::setPosition(int x, int y)
{
}

void Ball::incrementAngle(double a)
{
}
