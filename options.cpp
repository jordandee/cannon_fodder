#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gameengine.h"
#include "options.h"
#include "level.h"
#include "resources.h"

Options Options::_s;

void makeButton(SDL_Renderer *renderer, Button *button, const char *text, TTF_Font *font)
{
  SDL_Color text_color = {255,255,255};

  TTF_SizeText(font, text, &button->rect.w, &button->rect.h);

  SDL_Surface *text_surf = TTF_RenderText_Solid(font, text, text_color);
  button->texture = SDL_CreateTextureFromSurface(renderer, text_surf);
  SDL_FreeSurface(text_surf);
}

void setButtonPosition(Button *button, int x, int y)
{
  button->rect.x = x;
  button->rect.y = y;
}

void Options::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;
  outline = {0,0,0,0};
  option = 0;

  font48 = TTF_OpenFont("fonts/Chicago.ttf", 48);
  font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);

  title = {};
  terrain_type = {};
  obstacles ={};
  wind = {};
  fullscreen = {};

  makeButton(ge->renderer, &title, "Options", font48);
  setButtonPosition(&title, 100, 100);

  makeButton(ge->renderer, &terrain_type, "Terrain Type", font24);
  setButtonPosition(&terrain_type, 100, 250);

  makeButton(ge->renderer, &obstacles, "Obstacle Total", font24);
  setButtonPosition(&obstacles, 100, 300);

  makeButton(ge->renderer, &wind, "Wind", font24);
  setButtonPosition(&wind, 100, 350);

  makeButton(ge->renderer, &fullscreen, "FullScreen", font24);
  setButtonPosition(&fullscreen, 100, 400);
}

void Options::quit()
{
}

void Options::handleEvents(GameEngine* ge)
{
  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    if (e.type == SDL_QUIT)
      ge->stop();
    else if (e.type == SDL_KEYDOWN)
    {
      if (e.key.keysym.sym == SDLK_ESCAPE)
        ge->stop();
      else if (e.key.keysym.sym == SDLK_TAB)
      {
        SDL_Keymod keymod = SDL_GetModState();
        if (keymod == KMOD_LSHIFT || keymod == KMOD_RSHIFT || keymod == KMOD_SHIFT)
        {
          option--;
          if (option < 0)
            option = 3;
        }
        else
        {
          option++;
        }
      }
      else if (e.key.keysym.sym == SDLK_RETURN || e.key.keysym.sym == SDLK_SPACE)
      {
        //selectOption(ge);
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        //selectOption(ge);
      }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        int x = e.button.x;
        int y = e.button.y;
    }
  }
}

void Options::update()
{
}

void Options::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 0);
  //SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, title.texture, NULL, &title.rect);

  SDL_RenderCopy(ge->renderer, terrain_type.texture, NULL, &terrain_type.rect);
  SDL_RenderCopy(ge->renderer, obstacles.texture, NULL, &obstacles.rect);
  SDL_RenderCopy(ge->renderer, wind.texture, NULL, &wind.rect);
  SDL_RenderCopy(ge->renderer, fullscreen.texture, NULL, &fullscreen.rect);

  /*
  // draw outline 4 pixels wide
  SDL_RenderDrawRect(ge->renderer, &outline);
  outline.x += 1;
  outline.y += 1;
  outline.w -= 2;
  outline.h -= 2;
  SDL_RenderDrawRect(ge->renderer, &outline);
  outline.x += 1;
  outline.y += 1;
  outline.w -= 2;
  outline.h -= 2;
  SDL_RenderDrawRect(ge->renderer, &outline);
  outline.x += 1;
  outline.y += 1;
  outline.w -= 2;
  outline.h -= 2;
  SDL_RenderDrawRect(ge->renderer, &outline);
  */

  SDL_RenderPresent(ge->renderer);
}
