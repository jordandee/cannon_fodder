#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "score.h"

Score::Score()
{
  text_texture = NULL;
  font = NULL;
  text_surf = NULL;
  text_rect = {0,0,0,0};
  text_color = {0,0,0};
}

Score::~Score()
{
  SDL_DestroyTexture(text_texture);
  // TTF_CloseFont(font); // causes seg faults, why?
}

void Score::init(SDL_Renderer* renderer)
{
  font = TTF_OpenFont("fonts/Chicago.ttf", 12);

  char score[10] = "<-SCORE->";

  // calculate size of score text using font size specified
  TTF_SizeText(font, &score[0], &text_rect.w, &text_rect.h);
  text_rect.x = 400 - text_rect.w/2;
  text_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font, score, text_color);
  text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;
}

void Score::update(double dt)
{
}

void Score::render(SDL_Renderer* renderer)
{
  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
}
