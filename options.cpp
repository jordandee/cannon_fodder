#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
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

void CreateButtonsForLabel(SDL_Renderer *renderer, Label *label, int nButtons, char text[][20], TTF_Font *font)
{
  int pad = 40;
  label->valid_selections = nButtons;

  int x = 225; // fullscreen label width + pad
  int y = label->rect.y;

  for (int i = 0; i < nButtons; ++i)
  {
    makeButton(renderer, &label->selection[i], text[i], font);
    setButtonPosition(&label->selection[i], x, y);
    x += label->selection[i].rect.w + pad;
  }
}

void DrawButtonOutline(SDL_Renderer *renderer, SDL_Rect outline)
{
  int pad = 10;

  outline.x -= pad;
  outline.y -= pad;
  outline.w += 2*pad;
  outline.h += 2*pad;

  for (int i= 0; i < 4; ++i)
  {
    SDL_RenderDrawRect(renderer, &outline);
    outline.x += 1;
    outline.y += 1;
    outline.w -= 2;
    outline.h -= 2;
  }
}

void Options::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;
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

  makeButton(ge->renderer, &terrain_type.button, "Terrain", font24);
  setButtonPosition(&terrain_type.button, 50, 250);
  char terrain_settings[][20] = {"Flat","Smooth","Rough","Extreme","???"};
  CreateButtonsForLabel(ge->renderer, &terrain_type, 5, terrain_settings, font24);

  makeButton(ge->renderer, &obstacles.button, "Obstacles", font24);
  setButtonPosition(&obstacles.button, 50, 300);
  char obstacles_settings[][20] = {"None", "Few", "Many", "Lots", "???"};
  CreateButtonsForLabel(ge->renderer, &obstacles, 5, obstacles_settings, font24);

  makeButton(ge->renderer, &wind.button, "Wind", font24);
  setButtonPosition(&wind.button, 50, 350);
  char wind_settings[][20] = {"None", "Slow", "Fast","???"};
  CreateButtonsForLabel(ge->renderer, &wind, 4, wind_settings, font24);

  makeButton(ge->renderer, &fullscreen.button, "FullScreen", font24);
  setButtonPosition(&fullscreen.button, 50, 400);
  char fullscreen_settings[][20] = {"Enabled", "Disabled"};
  CreateButtonsForLabel(ge->renderer, &fullscreen, 2, fullscreen_settings, font24);
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
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, title.texture, NULL, &title.rect);

  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  for (int label_index = 0; label_index < 4; ++label_index)
  {
    SDL_RenderCopy(ge->renderer, labels[label_index].texture, NULL, &labels[label_index].rect);
    for (int i = 0; i < labels[label_index].valid_selections; ++i)
    {
      Button b = labels[label_index].selection[i];
      SDL_RenderCopy(ge->renderer, b.texture, NULL, &b.rect);
      DrawButtonOutline(ge->renderer, b.rect);
    }
  }

  SDL_RenderPresent(ge->renderer);
}
