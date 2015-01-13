#ifndef CANNON_H
#define CANNON_H

#include <SDL2/SDL.h>

#define CANNON_WIDTH 30
#define CANNON_HEIGHT 24

class Cannon
{
  public:
    Cannon();
    ~Cannon();

    void init(SDL_Renderer* renderer, bool flipped);
    void update();
    void render(SDL_Renderer* renderer);

    void setPosition(int x, int y);
    void incrementAngle(double a);

    int getCX();
    int getCY();
    double getAngle();

  private:
    SDL_Texture* cannon_texture;
    SDL_Rect cannon_rect;

    SDL_Texture* shaft_texture;
    SDL_Rect shaft_rect;
    double shaft_rotation_angle;
    SDL_Point shaft_rotation_point;

    bool is_flipped; // cannon on right is horizontally flipped
};

#endif
