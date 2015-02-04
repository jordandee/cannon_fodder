#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gameengine.h"
#include "title.h"
#include "level.h"
#include "resources.h"

Title Title::_s;

void Title::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;
  text_surf = NULL;
  text_color = {0,0,0};
  title_texture = NULL;
  title_rect = {0,0,0,0};
  vs_human_texture = NULL;
  vs_human_rect = {0,0,0,0};
  vs_bot_texture = NULL;
  vs_bot_rect = {0,0,0,0};
  options_texture = NULL;
  options_rect = {0,0,0,0};
  exit_texture = NULL;
  exit_rect = {0,0,0,0};

  // title center heading
  font48 = TTF_OpenFont("fonts/Chicago.ttf", 48);
  char title[20] = "Cannon Fodder";

  TTF_SizeText(font48, &title[0], &title_rect.w, &title_rect.h);
  title_rect.x = 400 - title_rect.w/2;
  title_rect.y = 128;

  text_surf = TTF_RenderText_Solid(font48, title, text_color);
  title_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  // menu
  text_color = {255,255,255};
  font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);
  char vs_human_button[20] = "vs Human";

  // calculate size of score text using font size specified
  TTF_SizeText(font24, &vs_human_button[0], &vs_human_rect.w, &vs_human_rect.h);
  vs_human_rect.x = 500;
  vs_human_rect.y = 350;

  text_surf = TTF_RenderText_Solid(font48, vs_human_button, text_color);
  vs_human_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;
  
  char vs_bot_button[20] = "vs Bot";

  // calculate size of score text using font size specified
  TTF_SizeText(font24, &vs_bot_button[0], &vs_bot_rect.w, &vs_bot_rect.h);
  vs_bot_rect.x = 500;
  vs_bot_rect.y = 400;

  text_surf = TTF_RenderText_Solid(font48, vs_bot_button, text_color);
  vs_bot_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  char options_button[20] = "Options";

  // calculate size of score text using font size specified
  TTF_SizeText(font24, &options_button[0], &options_rect.w, &options_rect.h);
  options_rect.x = 500;
  options_rect.y = 450;

  text_surf = TTF_RenderText_Solid(font48, options_button, text_color);
  options_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;

  char exit_button[20] = "Exit";

  // calculate size of score text using font size specified
  TTF_SizeText(font24, &exit_button[0], &exit_rect.w, &exit_rect.h);
  exit_rect.x = 500;
  exit_rect.y = 500;

  text_surf = TTF_RenderText_Solid(font48, exit_button, text_color);
  exit_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
  SDL_FreeSurface(text_surf);
  text_surf = NULL;
}

void Title::quit()
{
}

void Title::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      ge->stop();
    if (e.type == SDL_KEYDOWN)
    {
      if (e.key.keysym.sym == SDLK_ESCAPE)
        ge->stop();
      else
        ge->changeState(Level::Instance());
    }
  }
}

void Title::update()
{
}

void Title::render(GameEngine* ge)
{
  //SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 0);
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, background_texture, NULL, &background_rect);
  SDL_RenderCopy(ge->renderer, title_texture, NULL, &title_rect);

  SDL_RenderCopy(ge->renderer, vs_human_texture, NULL, &vs_human_rect);
  SDL_RenderCopy(ge->renderer, vs_bot_texture, NULL, &vs_bot_rect);
  SDL_RenderCopy(ge->renderer, options_texture, NULL, &options_rect);
  SDL_RenderCopy(ge->renderer, exit_texture, NULL, &exit_rect);

  SDL_RenderPresent(ge->renderer);
}
