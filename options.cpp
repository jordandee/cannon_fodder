#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "gameengine.h"
#include "options.h"
#include "level.h"
#include "resources.h"
#include "buttons.h"
#include "title.h"
#include "globals.h"

Options Options::_s;

void Options::init(GameEngine* ge)
{
  background_rect = {0, 0, 800, 600};
  background_texture = NULL;
  background_texture = loadTexture("images/screenshot0.png", ge->renderer);

  font48 = NULL;
  font24 = NULL;
  font48 = TTF_OpenFont("fonts/Chicago.ttf", 48);
  font24 = TTF_OpenFont("fonts/Chicago.ttf", 24);

  text_color = {255,255,255};

  mouse_moved = false;
  mouseX = 0;
  mouseY = 0;

  title = {};
  terrain_type = {};
  obstacles ={};
  wind = {};
  fullscreen = {};
  highlighted_label = 0;
  highlighted_selection = 0;
  highlight_enable = false;
  terrain_type.active_selection = gTerrainOption;
  obstacles.active_selection = gObstaclesOption;
  wind.active_selection = gWindOption;
  fullscreen.active_selection = gFullScreenOption;
  exit.active_selection = 0;

  makeButton(ge->renderer, &title, "Options", font48, text_color);
  setButtonPosition(&title, 100, 100);

  makeButton(ge->renderer, &terrain_type.button, "Terrain", font24, text_color);
  setButtonPosition(&terrain_type.button, 50, 250);
  char terrain_settings[][20] = {"Flat","Smooth","Rough","Extreme","???"};
  CreateButtonsForLabel(ge->renderer, &terrain_type, 5, terrain_settings, font24, text_color);

  makeButton(ge->renderer, &obstacles.button, "Obstacles", font24, text_color);
  setButtonPosition(&obstacles.button, 50, 300);
  char obstacles_settings[][20] = {"None", "Few", "Many", "Lots", "???"};
  CreateButtonsForLabel(ge->renderer, &obstacles, 5, obstacles_settings, font24, text_color);

  makeButton(ge->renderer, &wind.button, "Wind", font24, text_color);
  setButtonPosition(&wind.button, 50, 350);
  char wind_settings[][20] = {"None", "Slow", "Fast","???"};
  CreateButtonsForLabel(ge->renderer, &wind, 4, wind_settings, font24, text_color);

  makeButton(ge->renderer, &fullscreen.button, "Fullscreen", font24, text_color);
  setButtonPosition(&fullscreen.button, 50, 400);
  char fullscreen_settings[][20] = {"Disabled", "Enabled"};
  CreateButtonsForLabel(ge->renderer, &fullscreen, 2, fullscreen_settings, font24, text_color);
  
  makeButton(ge->renderer, &exit.button, "Exit and ->", font24, text_color);
  setButtonPosition(&exit.button, 50, 500);
  char exit_settings[][20] = {"Save", "Cancel"};
  CreateButtonsForLabel(ge->renderer, &exit, 2, exit_settings, font24, text_color);
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
      SDL_Keycode key = e.key.keysym.sym;
      if (key == SDLK_ESCAPE)
      {
        ge->changeState(Title::Instance());
      }

      if (!highlight_enable)
      {
        highlight_enable = true;
        break;
      }

      if (key == SDLK_TAB)
      {
        SDL_Keymod keymod = SDL_GetModState();
        if (keymod == KMOD_LSHIFT || keymod == KMOD_RSHIFT || keymod == KMOD_SHIFT)
        {
          highlighted_selection--;
          if (highlighted_selection < 0)
            highlighted_selection = labels[highlighted_label].valid_selections - 1;
        }
        else
        {
          highlighted_selection++;
          if (highlighted_selection >= labels[highlighted_label].valid_selections)
            highlighted_selection = 0;
        }
      }
      else if (key == SDLK_RETURN || key == SDLK_SPACE ||
               key == SDLK_KP_0 || key == SDLK_KP_ENTER)
      {
        if (highlight_enable)
        {
          labels[highlighted_label].active_selection = highlighted_selection;

          // exit: save or cancel
          if (highlighted_label == 4)
          {
            if (exit.active_selection == 0)
            {
              gTerrainOption = terrain_type.active_selection;
              gObstaclesOption = obstacles.active_selection;
              gWindOption = wind.active_selection;
              gFullScreenOption = fullscreen.active_selection;

              saveSettings();
            }
            ge->changeState(Title::Instance());
          }
        }
      }
      else if (key == SDLK_w || key == SDLK_UP)
      {
        highlighted_label--;
        if (highlighted_label < 0)
          highlighted_label = total_labels - 1;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;
      }
      else if (key == SDLK_s || key == SDLK_DOWN)
      {
        highlighted_label++;
        if (highlighted_label >= total_labels)
          highlighted_label = 0;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;

      }
      else if (key == SDLK_a || key == SDLK_LEFT)
      {
        highlighted_selection--;
        if (highlighted_selection < 0)
          highlighted_selection = labels[highlighted_label].valid_selections - 1;
      }
      else if (key == SDLK_d || key == SDLK_RIGHT)
      {
        highlighted_selection++;
        if (highlighted_selection >= labels[highlighted_label].valid_selections)
          highlighted_selection = 0;
      }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN)
    {
      if (e.button.button == SDL_BUTTON_LEFT)
      {
        if (highlight_enable)
        {
          labels[highlighted_label].active_selection = highlighted_selection;
          
          // exit: save or cancel
          if (highlighted_label == 4)
          {
            if (exit.active_selection == 0)
            {
              gTerrainOption = terrain_type.active_selection;
              gObstaclesOption = obstacles.active_selection;
              gWindOption = wind.active_selection;
              gFullScreenOption = fullscreen.active_selection;

              saveSettings();
            }
            ge->changeState(Title::Instance());
          }
        }
      }
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
      mouse_moved = true;
      mouseX = e.button.x;
      mouseY = e.button.y;
    }
  }
}

void Options::update()
{
  if (mouse_moved)
  {
    highlight_enable = false;
    for (int label_index = 0; label_index < total_labels; ++label_index)
    {
      for (int i = 0; i < labels[label_index].valid_selections; ++i)
      {
        Button b = labels[label_index].selection[i];
        if (checkPointRectCollision(mouseX, mouseY, b.rect))
        {
          highlighted_label = label_index;
          highlighted_selection = i;
          highlight_enable = true;
        }
      }
    }
    mouse_moved = false;
  }
}

void Options::render(GameEngine* ge)
{
  SDL_SetRenderDrawColor(ge->renderer, 0, 0, 0, 0);
  SDL_RenderClear(ge->renderer);

  SDL_RenderCopy(ge->renderer, title.texture, NULL, &title.rect);

  SDL_SetRenderDrawColor(ge->renderer, 0xff, 0xff, 0xff, 0);
  for (int label_index = 0; label_index < total_labels; ++label_index)
  {
    SDL_RenderCopy(ge->renderer, labels[label_index].texture, NULL, &labels[label_index].rect);
    for (int i = 0; i < labels[label_index].valid_selections; ++i)
    {
      Button b = labels[label_index].selection[i];
      SDL_RenderCopy(ge->renderer, b.texture, NULL, &b.rect);
      if (labels[label_index].active_selection == i)
      {
        // Draw Selection outline for all options except save/cancel
        if (label_index != total_labels-1)
        {
          DrawButtonOutline(ge->renderer, b.rect, 10, 4);
        }
      }
    }
  }
  if (highlight_enable)
    DrawButtonOutline(ge->renderer, labels[highlighted_label].selection[highlighted_selection].rect, 5, 2);

  SDL_RenderPresent(ge->renderer);
}
