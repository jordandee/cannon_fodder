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

    void playerLScores(int points);
    void playerRScores(int points);
    void updateScoreTextures(SDL_Renderer* renderer);

  private:
    int scoreL, scoreR;
    SDL_Texture *text_texture, *scoreL_texture, *scoreR_texture;
    TTF_Font *font;
    SDL_Surface *text_surf;
    SDL_Rect text_rect, scoreL_rect, scoreR_rect;
    SDL_Color text_color;

    bool update_score_textures;
};

#endif
