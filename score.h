#ifndef SCORE_H
#define SCORE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

class Score
{
  public:
    Score();
    ~Score();

    void init(SDL_Renderer* renderer);
    void update(double dt);
    void render(SDL_Renderer* renderer);

  private:
    SDL_Texture* text_texture;
    TTF_Font *font;
    SDL_Surface* text_surf;
    SDL_Rect text_rect;
    SDL_Color text_color;
};

#endif
