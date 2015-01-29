#include <SDL2/SDL.h>
#include <iostream>
#include "resources.h"
#include "obstacle.h"

Obstacle::Obstacle()
{
  obstacle_texture = NULL;
}

Obstacle::~Obstacle()
{
  SDL_DestroyTexture(obstacle_texture);
}

void Obstacle::init(SDL_Renderer* renderer, bool flipped)
{
  alive = true;

  is_flipped = flipped;

  obstacle_texture = loadTexture("images/hospital.png", renderer);

  obstacle_rect.w = HOSPITAL_WIDTH;
  obstacle_rect.h = HOSPITAL_HEIGHT;
}

void Obstacle::update()
{
}

void Obstacle::render(SDL_Renderer* renderer)
{
  if (alive)
  {
    if (!is_flipped)
    {
      SDL_RenderCopy(renderer, obstacle_texture, NULL, &obstacle_rect);
    }
    else
    {
      SDL_RenderCopyEx(renderer, obstacle_texture, NULL, &obstacle_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
  }
}

void Obstacle::setPosition(int x, int y)
{
  obstacle_rect.x = x;
  obstacle_rect.y = y;

  //std::cout << "obs rect: " << x << " " << y << " " << obstacle_rect.w << " " << obstacle_rect.h << "\n";
  //std::cout << "alive flipped: " << alive << " " << is_flipped << "\n";
  //std::cout << "texture: " << obstacle_texture << "\n";
}

int Obstacle::getCX()
{
  return obstacle_rect.x + (HOSPITAL_WIDTH/2);
}

int Obstacle::getCY()
{
  return obstacle_rect.y + (HOSPITAL_HEIGHT/2);
}

SDL_Rect* Obstacle::getRect()
{
  return &obstacle_rect;
}
