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

  text_texture = NULL;
  font = NULL;
  text_surf = NULL;
  text_rect = {0,0,0,0};
  text_color = {0,0,0};

  font = TTF_OpenFont("fonts/Chicago.ttf", 48);

  // score center heading
  char title[20] = "Cannon Fodder";

  // calculate size of score text using font size specified
  TTF_SizeText(font, &title[0], &text_rect.w, &text_rect.h);
  text_rect.x = 400 - text_rect.w/2;
  text_rect.y = 128;

  text_surf = TTF_RenderText_Solid(font, title, text_color);
  text_texture = SDL_CreateTextureFromSurface(ge->renderer, text_surf);
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
  SDL_RenderCopy(ge->renderer, text_texture, NULL, &text_rect);

  SDL_RenderPresent(ge->renderer);
}
