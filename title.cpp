#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "gameengine.h"
#include "title.h"
#include "level.h"
#include "resources.h"
#include "options.h"
#include "globals.h"

Title Title::_s;

void Title::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;
  font48 = TTF_OpenFont("fonts/Chicago.ttf", 48);
  font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);

  // title center heading
  text_color = {0,0,0};

  bTitle = {};
  makeButton(ge->renderer, &bTitle, "Cannon Fodder", font48, text_color);
  setButtonPosition(&bTitle, 400 - bTitle.rect.w/2, 128);

  // menu
  text_color = {255,255,255};

  vs_human = {};
  makeButton(ge->renderer, &vs_human, "vs Human", font24, text_color);
  setButtonPosition(&vs_human, 500, 350);

  vs_bot = {};
  makeButton(ge->renderer, &vs_bot, "vs Bot", font24, text_color);
  setButtonPosition(&vs_bot, 500, 400);

  options = {};
  makeButton(ge->renderer, &options, "Options", font24, text_color);
  setButtonPosition(&options, 500, 450);

  exit = {};
  makeButton(ge->renderer, &exit, "Exit", font24, text_color);
  setButtonPosition(&exit, 500, 500);
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
    else if (e.type == SDL_KEYDOWN)
    {
      SDL_Keycode key = e.key.keysym.sym;
      if (key == SDLK_ESCAPE)
      {
        ge->stop();
      }
      else if (key == SDLK_w || key == SDLK_UP)
      {
        option--;
        if (option < 0)
          option = 3;
      }
      else if (key == SDLK_s || key == SDLK_DOWN)
      {
        option++;
        if (option > 3)
          option = 0;
      }
      else if (key == SDLK_TAB)
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
          if (option > 3)
            option = 0;
        }
      }
      else if (key == SDLK_RETURN || key == SDLK_SPACE || 
               key == SDLK_KP_0 || key == SDLK_KP_ENTER)
      {
        selectOption(ge);
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        selectOption(ge);
      }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        int x = e.button.x;
        int y = e.button.y;
        if (x > 490 && x < (490+vs_human.rect.w+20))
        {
          if (y > 340)
          {
            if (y < 390)
              option = 0;
            else if (y < 440)
              option = 1;
            else if (y < 490)
              option = 2;
            else if (y < 540)
              option = 3;
          }
        }
    }
  }
}

void Title::update()
{
}

void Title::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, background_texture, NULL, &background_rect);
  SDL_RenderCopy(ge->renderer, bTitle.texture, NULL, &bTitle.rect);

  SDL_RenderCopy(ge->renderer, vs_human.texture, NULL, &vs_human.rect);
  SDL_RenderCopy(ge->renderer, vs_bot.texture, NULL, &vs_bot.rect);
  SDL_RenderCopy(ge->renderer, options.texture, NULL, &options.rect);
  SDL_RenderCopy(ge->renderer, exit.texture, NULL, &exit.rect);

  // draw outline 4 pixels wide
  DrawButtonOutline(ge->renderer, buttons[option].rect, 10, 4);

  SDL_RenderPresent(ge->renderer);
}

void Title::selectOption(GameEngine* ge)
{
  switch (option)
  {
    case 0:
      {
        ge->changeState(Level::Instance());
        break;
      }
    case 1:
      {
        gAI_Enable = true;
        ge->changeState(Level::Instance());
        break;
      }
    case 2:
      {
        ge->changeState(Options::Instance());
        break;
      }
    case 3:
      {
        ge->stop();
        break;
      }
    default:
      {
        break;
      }
  }
}
