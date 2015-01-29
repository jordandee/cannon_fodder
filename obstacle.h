#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>

#define HOSPITAL_WIDTH 28
#define HOSPITAL_HEIGHT 28

class Obstacle
{
  public:
    Obstacle();
    ~Obstacle();

    void init(SDL_Renderer* renderer, bool flipped);
    void update();
    void render(SDL_Renderer* renderer);

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
