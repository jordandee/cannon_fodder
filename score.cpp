#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <cstring>
#include "score.h"

Score::Score()
{
  text_texture = NULL;
  scoreL_texture = NULL;
  scoreR_texture = NULL;
  battleL_texture = NULL;
  battleR_texture = NULL;
  warL_texture = NULL;
  warR_texture = NULL;
  font12 = NULL;
  font24 = NULL;
  text_surf = NULL;
  text_rect = {0,0,0,0};
  scoreL_rect = {0,0,0,0};
  scoreR_rect = {0,0,0,0};
  battle_rect = {0,0,0,0};
  war_rect = {0,0,0,0};
  text_color = {0,0,0};
}

Score::~Score()
{
  SDL_DestroyTexture(text_texture);
  // TTF_CloseFont(font12); // causes seg faults, why?
}

void Score::init(SDL_Renderer* renderer)
{
  scoreL = 0;
  scoreR = 0;

  battleMessage = 0;
  warMessage = 0;
  
  if (!font12)
  {
    font12 = TTF_OpenFont("fonts/Chicago.ttf", 12);
  }
  if (!font24)
  {
    font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);
  }

  updateScoreTextures(renderer);
  update_score_textures = false;

  if (!text_texture)
  {
    // score center heading
    char score[10] = "<-SCORE->";

    // calculate size of score text using font size specified
    TTF_SizeText(font12, &score[0], &text_rect.w, &text_rect.h);
    text_rect.x = 400 - text_rect.w/2;
    text_rect.y = 5;

    text_surf = TTF_RenderText_Solid(font12, score, text_color);
    text_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    text_surf = NULL;
  }


  // win a battle messages
  if (!battleL_texture)
  {
    char battlewon1[30] = "Player 1 has won the battle!!";

    TTF_SizeText(font24, &battlewon1[0], &battle_rect.w, &battle_rect.h);
    battle_rect.x = 400 - battle_rect.w/2;
    battle_rect.y = 100;

    text_surf = TTF_RenderText_Solid(font24, battlewon1, text_color);
    battleL_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    text_surf = NULL;
  }
  if (!battleR_texture)
  {
    char battlewon2[30] = "Player 2 has won the battle!!";

    TTF_SizeText(font24, &battlewon2[0], &battle_rect.w, &battle_rect.h);
    battle_rect.x = 400 - battle_rect.w/2;
    battle_rect.y = 100;

    text_surf = TTF_RenderText_Solid(font24, battlewon2, text_color);
    battleR_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    text_surf = NULL;
  }

  // win a war messages
  if (!warL_texture)
  {
    char warwon1[30] = "Player 1 has won the WAR!!!";

    TTF_SizeText(font24, &warwon1[0], &war_rect.w, &war_rect.h);
    war_rect.x = 400 - war_rect.w/2;
    war_rect.y = 100;

    text_surf = TTF_RenderText_Solid(font24, warwon1, text_color);
    warL_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    text_surf = NULL;
  }

  if (!warR_texture)
  {
    char warwon2[30] = "Player 2 has won the WAR!!!";
    
    TTF_SizeText(font24, &warwon2[0], &war_rect.w, &war_rect.h);
    war_rect.x = 400 - war_rect.w/2;
    war_rect.y = 100;

    text_surf = TTF_RenderText_Solid(font24, warwon2, text_color);
    warR_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
    SDL_FreeSurface(text_surf);
    text_surf = NULL;
  }
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

  if (battleMessage == 1)
    SDL_RenderCopy(renderer, battleL_texture, NULL, &battle_rect);
  else if (battleMessage == 2)
    SDL_RenderCopy(renderer, battleR_texture, NULL, &battle_rect);

  if (warMessage == 1)
    SDL_RenderCopy(renderer, warL_texture, NULL, &war_rect);
  else if (warMessage == 2)
    SDL_RenderCopy(renderer, warR_texture, NULL, &war_rect);
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

  TTF_SizeText(font12, &score[0], &scoreL_rect.w, &scoreL_rect.h);
  scoreL_rect.x = 200 - scoreL_rect.w/2;
  scoreL_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font12, score, text_color);
  scoreL_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  // right score
  score_str = std::to_string(scoreR);
  std::strcpy(&score[0], score_str.c_str());

  TTF_SizeText(font12, &score[0], &scoreR_rect.w, &scoreR_rect.h);
  scoreR_rect.x = 600 - scoreR_rect.w/2;
  scoreR_rect.y = 5;

  text_surf = TTF_RenderText_Solid(font12, score, text_color);
  scoreR_texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;
}

int Score::getPlayerLScore(void)
{
  return scoreL;
}

int Score::getPlayerRScore(void)
{
  return scoreR;
}

void Score::resetScores(void)
{
  update_score_textures = true;
  scoreL = 0;
  scoreR = 0;
}
