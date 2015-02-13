#include <SDL2/SDL.h>
#include "resources.h"
#include "cannon.h"

Cannon::Cannon()
{
  cannon_texture = NULL;
  shaft_texture = NULL;
}

Cannon::~Cannon()
{
  SDL_DestroyTexture(cannon_texture);
  SDL_DestroyTexture(shaft_texture);
}

void Cannon::init(SDL_Renderer* renderer, bool flipped)
{
  alive = true;

  is_flipped = flipped;

  cannon_texture = loadTexture("images/cannon_wheel.png", renderer);

  cannon_rect.w = CANNON_WIDTH;
  cannon_rect.h = CANNON_HEIGHT;

  shaft_texture = loadTexture("images/cannon_shaft.png", renderer);

  shaft_rect.w = 4;
  shaft_rect.h = 14;

  shaft_rotation_angle = 0.0;
  shaft_rotation_point.x = shaft_rect.w/2;
  shaft_rotation_point.y = shaft_rect.h-2;
}

void Cannon::update()
{
}

void Cannon::render(SDL_Renderer* renderer)
{
  if (alive)
  {
    if (!is_flipped)
    {
      SDL_RenderCopy(renderer, cannon_texture, NULL, &cannon_rect);
    }
    else
    {
      SDL_RenderCopyEx(renderer, cannon_texture, NULL, &cannon_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }

    SDL_RenderCopyEx(renderer, shaft_texture, NULL, &shaft_rect, shaft_rotation_angle, &shaft_rotation_point, SDL_FLIP_NONE);
  }
}

void Cannon::setPosition(int x, int y)
{
  cannon_rect.x = x;
  cannon_rect.y = y;

  if (!is_flipped)
  {
    shaft_rect.x = cannon_rect.x + 17;
    shaft_rect.y = cannon_rect.y + 3;
  }
  else
  {
    shaft_rect.x = cannon_rect.x + 9;
    shaft_rect.y = cannon_rect.y + 3;
  }
}

void Cannon::incrementAngle(double a)
{
  shaft_rotation_angle += a;
}

int Cannon::getCX()
{
  return cannon_rect.x + (CANNON_WIDTH/2);
}

int Cannon::getCY()
{
  return cannon_rect.y + (CANNON_HEIGHT/2);
}

void Cannon::setAngle(double angle)
{
  shaft_rotation_angle = angle;
}

double Cannon::getAngle()
{
  return shaft_rotation_angle;
}

SDL_Rect* Cannon::getRect()
{
  return &cannon_rect;
}

void Cannon::die()
{
  alive = false;
}
void Cannon::live()
{
  alive = true;
}
