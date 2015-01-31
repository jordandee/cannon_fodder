#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "resources.h"
#include "math.h"
#include "terrain.h"
#include "obstacle.h"

Obstacle::Obstacle()
{
  obstacle_texture = NULL;
}

Obstacle::~Obstacle()
{
  SDL_DestroyTexture(obstacle_texture);
}

void Obstacle::init(SDL_Renderer* renderer, Obstacle_Type obs_type, bool flipped)
{
  type = obs_type;
  alive = true;
  is_flipped = flipped;

  if (type == HOSPITAL)
  {
    obstacle_texture = loadTexture("images/hospital.png", renderer);
    obstacle_rect.w = HOSPITAL_WIDTH;
    obstacle_rect.h = HOSPITAL_HEIGHT;
  }
  else if (type == HOUSE)
  {
    obstacle_texture = loadTexture("images/house.png", renderer);
    obstacle_rect.w = HOUSE_WIDTH;
    obstacle_rect.h = HOUSE_HEIGHT;
  }
  else if (type == TREE)
  {
    obstacle_texture = loadTexture("images/tree.png", renderer);
    obstacle_rect.w = TREE_WIDTH;
    obstacle_rect.h = TREE_HEIGHT;
  }
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

void Obstacle::findPosition(std::vector<Pixel>& terrain, std::vector<SDL_Rect *> rects)
{
  bool position_found = false;

  int x;
  while (!position_found)
  {
    if (is_flipped == !FLIPPED)
      x = nrand(400 - obstacle_rect.w);
    else
      x = 400 + nrand(400 - obstacle_rect.w);

    position_found = true;
    for (auto it = rects.begin(); it != rects.end(); ++it)
    {
      if (!((*it)->x + (*it)->w < x || x + obstacle_rect.w < (*it)->x))
        position_found &= false;
    }
  }
  int y = findTopGroundPixel(terrain, x+obstacle_rect.w/2);

  setPosition(x, y - obstacle_rect.h);
  fixTerrain(terrain, &obstacle_rect);
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
