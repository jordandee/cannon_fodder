#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

#define BALL_WIDTH 4
#define BALL_HEIGHT 4

class Ball
{
  public:
    Ball();
    ~Ball();

    void init(SDL_Renderer* renderer);
    void update(double dt);
    void render(SDL_Renderer* renderer);

    void setPosition(int x, int y);
    void incrementAngle(double a);

    void shoot(int cannon_cx, int cannon_cy, double shot_dt, double shot_angle);

  private:
    SDL_Texture* ball_texture;
    SDL_Rect ball_rect;

    double ball_angle;
    SDL_Point ball_rotation_point;

    double x, y, vx, vy, ax, ay; // position, velocity, acceleration
};

#endif
