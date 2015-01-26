#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstring>
#include "score.h"

Score::Score()
{
  scoreL = 1337;
  scoreR = 123456789;
  text_texture = NULL;
  scoreL_texture = NULL;
  scoreR_texture = NULL;
  font = NULL;
  text_surf = NULL;
  text_rect = {0,0,0,0};
  scoreL_rect = {0,0,0,0};
  scoreR_rect = {0,0,0,0};
  text_color = {0,0,0};
  update_score_textures = false;
}

Score::~Score()
{
  SDL_DestroyTexture(text_texture);
  // TTF_CloseFont(font); // causes seg faults, why?
}

void Score::init(SDL_Renderer* renderer)
{
  font = TTF_OpenFont("fonts/Chicago.ttf", 12);

  // score center heading
  char score[10] = "<-SCORE->";

  // calculate size of score text using font size specified
  TTF_SizeText(font, &score[0], &text_rect.w, &text_rect.h);
  text_rect.x = 400 - text_rect.w/2;
  text_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font, score, text_color);
  text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  updateScoreTextures(renderer);
}

void Score::update(double dt)
{
}

void Score::render(SDL_Renderer* renderer)
{
  if (update_score_textures)
  {
    updateScoreTextures(renderer);
    update_score_textures = false;
  }

  SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);
  SDL_RenderCopy(renderer, scoreL_texture, NULL, &scoreL_rect);
  SDL_RenderCopy(renderer, scoreR_texture, NULL, &scoreR_rect);
}

void Score::playerLScores(int points)
{
  scoreL += points;

  update_score_textures = true;
}

void Score::playerRScores(int points)
{
  scoreR += points;

  update_score_textures = true;
}

void Score::updateScoreTextures(SDL_Renderer* renderer)
{
  char score[10];

  // left score
  std::string score_str = std::to_string(scoreL);
  std::strcpy(&score[0], score_str.c_str());

  TTF_SizeText(font, &score[0], &scoreL_rect.w, &scoreL_rect.h);
  scoreL_rect.x = 200 - scoreL_rect.w/2;
  scoreL_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font, score, text_color);
  scoreL_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  // right score
  score_str = std::to_string(scoreR);
  std::strcpy(&score[0], score_str.c_str());

  TTF_SizeText(font, &score[0], &scoreR_rect.w, &scoreR_rect.h);
  scoreR_rect.x = 600 - scoreR_rect.w/2;
  scoreR_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font, score, text_color);
  scoreR_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;
}
