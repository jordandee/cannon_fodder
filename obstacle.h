#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>
#include <vector>
#include "terrain.h"

#define HOSPITAL_WIDTH 28
#define HOSPITAL_HEIGHT 28
#define HOUSE_WIDTH 20
#define HOUSE_HEIGHT 20

#define FLIPPED true
enum Obstacle_Type
{
  HOSPITAL,
  HOUSE,
  TREE
};

class Obstacle
{
  public:
    Obstacle();
    ~Obstacle();

    void init(SDL_Renderer* renderer, Obstacle_Type type, bool flipped);
    void update();
    void render(SDL_Renderer* renderer);

    void findPosition(std::vector<Pixel>& terrain,
                      std::vector<SDL_Rect *> rects);
    void setPosition(int x, int y);

    int getCX();
    int getCY();
    SDL_Rect* getRect();

    bool alive;

  private:

    SDL_Texture* obstacle_texture;
    SDL_Rect obstacle_rect;

    bool is_flipped;
};

#endif
